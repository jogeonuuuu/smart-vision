#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include <cmath>
#include "dxl.hpp"

using namespace std;
using namespace cv;

bool ctrl_c_pressed = false;
void ctrlc(int) { ctrl_c_pressed = true; }

class LeftInfo {
private:
    int index;
    double distance;
public:
    LeftInfo(int index, double distance) {
        this->index = index;
        this->distance = distance;
    }
    int get_index(void) {
        return this->index;
    }
    double get_distance() {
        return this->distance;
    }
};

class RightInfo {
private:
    int index;
    double distance;
public:
    RightInfo(int index, double distance) {
        this->index = index;
        this->distance = distance;
    }
    int get_index(void) {
        return this->index;
    }
    double get_distance() {
        return this->distance;
    }
};

bool compare_left_info(LeftInfo &a, LeftInfo &b) {
    return a.get_distance() < b.get_distance();
}

bool compare_right_info(RightInfo &a, RightInfo &b) {
    return a.get_distance() < b.get_distance();
}

int main(void) {
    signal(SIGINT, ctrlc);
    Dxl dxl;

    string src = "nvarguscamerasrc sensor-id=0 ! \
        video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
        format=(string)NV12, framerate=(fraction)30/1 ! \
        nvvidconv flip-method=0 ! video/x-raw, \
        width=(int)640, height=(int)360, format=(string)BGRx ! \
        videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    VideoCapture source(src, CAP_GSTREAMER);
    if (!source.isOpened()) {
        cout << "Camera error" << endl;
        return -1;
    }

    string dst1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! \
        udpsink host=203.234.58.165 port=8001 sync=false";
    string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! \
        udpsink host=203.234.58.165 port=8002 sync=false";
    VideoWriter writer1(dst1, 0, (double)30, Size(640, 360), true);
    VideoWriter writer2(dst2, 0, (double)30, Size(640, 90), true);
    if (!writer1.isOpened() || !writer2.isOpened()) {
        cerr << "Writer open failed!" << endl;
        return -1;
    }

    Mat frame, toUse;
    Mat labels, stats, centroids;

    Point2d left_pnt(160, 45), right_pnt(480, 45);
    Point2d left_past_pnt = left_pnt, right_pnt_past = right_pnt;

    while (true) {
        source >> frame;
        if (frame.empty()) {
            cerr << "frame empty!" << endl;
            break;
        }
        if (ctrl_c_pressed) break;

        toUse = frame(Rect(0, frame.rows * 3 / 4, frame.cols, frame.rows / 4));
        cvtColor(toUse, toUse, COLOR_BGR2GRAY);
        toUse += 100 - mean(toUse)[0];
        threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU);
        int label_cnt = connectedComponentsWithStats(toUse, labels, stats, centroids);
        cvtColor(toUse, toUse, COLOR_GRAY2BGR);

        vector<LeftInfo> left_vec;
        vector<RightInfo> right_vec;

        for (int i = 1; i < label_cnt; i++) { // '0'은 배경
            double *p = centroids.ptr<double>(i);
            int *q = stats.ptr<int>(i);

            if (q[4] > 100) {
                double left_dist = sqrt(pow((left_pnt.x - p[0]), 2) + pow((left_pnt.y - p[1]), 2));
                double right_dist = sqrt(pow((right_pnt.x - p[0]), 2) + pow((right_pnt.y - p[1]), 2));
                left_vec.emplace_back(LeftInfo(i, left_dist));
                right_vec.emplace_back(RightInfo(i, right_dist));
            }
        }

        sort(left_vec.begin(), left_vec.end(), compare_left_info);
        sort(right_vec.begin(), right_vec.end(), compare_right_info);

        if (!left_vec.empty()) {
            double *lp = centroids.ptr<double>(left_vec[0].get_index());
            left_pnt = Point2d(lp[0], lp[1]);
        }
        if (!right_vec.empty()) {
            double *rp = centroids.ptr<double>(right_vec[0].get_index());
            right_pnt = Point2d(rp[0], rp[1]);
        }

        // 이동 거리 제한으로 좌표 보정
        if (abs(left_pnt.x - left_past_pnt.x) > toUse.cols / 4 ||
            abs(left_pnt.y - left_past_pnt.y) > toUse.rows / 2) {
            left_pnt = left_past_pnt;
        }
        if (abs(right_pnt.x - right_pnt_past.x) > toUse.cols / 4 ||
            abs(right_pnt.y - right_pnt_past.y) > toUse.rows / 2) {
            right_pnt = right_pnt_past;
        }

        // 따라갈 점 계산
        Point2d follow_pnt((left_pnt.x + right_pnt.x) / 2, (left_pnt.y + right_pnt.y) / 2);
        int error = toUse.cols / 2 - follow_pnt.x;
        int left_vel = (100 - (0.14 * error));
        int right_vel = -(100 + (0.14 * error));
        dxl.setVelocity(left_vel, right_vel);

        cout << "Error: " << error << ", Follow Point: " << follow_pnt << endl;

        // 시각화
        circle(toUse, left_pnt, 3, Scalar(255, 0, 0), -1); // 왼쪽 점
        circle(toUse, right_pnt, 3, Scalar(0, 255, 0), -1); // 오른쪽 점
        circle(toUse, follow_pnt, 3, Scalar(0, 0, 255), -1); // 따라갈 점

        writer1 << frame;
        writer2 << toUse;

        left_past_pnt = left_pnt;
        right_pnt_past = right_pnt;

        if (waitKey(30) == 27) break; // ESC 키로 종료
    }

    return 0;
}

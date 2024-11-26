#include <iostream>
#include <unistd.h>
#include <signal.h> //'ctrl+c'
#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;
 
bool ctrl_c_pressed = false;
void ctrlc(int) { ctrl_c_pressed = true; }

class Info {
private:
    int index;
    double distance;
public:
    Info(int index, double distance) {
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

bool compare_info(Info &a, Info &b) { //오름차순
    return a.get_distance() < b.get_distance();
}


int main(void)
{
    signal(SIGINT, ctrlc); // SIGINT 신호를 잡아내는 시그널 핸들러 설정
 
    /*string src = "nvarguscamerasrc sensor-id=0 ! \
        video/x-raw(memory:NVMM), width=(int)640, height=(int)360, \
        format=(string)NV12 ! nvvidconv flip-method=0 ! video/x-raw, \
        width=(int)640, height=(int)360, format=(string)BGRx ! \
        videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    VideoCapture source(src, CAP_GSTREAMER);*/
    //string src = "/home/jetson/jogeonuuuu/line_tracer/5_lt_cw_100rpm_out.mp4";
    string src = "/home/jetson/jogeonuuuu/line_tracer/7_lt_ccw_100rpm_in.mp4";
    VideoCapture source(src, CAP_FFMPEG);
    if (!source.isOpened()) { 
        cout << "Camera error" << endl; return -1; 
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
    if (!writer1.isOpened() && !writer2.isOpened()) { 
        cerr << "Writer open failed!" << endl; return -1;
    }
 
    Mat frame, toUse;
    Mat labels, stats, centroids;
 
    Point2d past_point(320, 45), present_point(320, 45); //과거 및 현재 좌표
    double distance; //중점(보드으로부터 가장 가까운 객체와의 거리

    struct timeval start,end1;
    double time1;
    while(true)
    {
        gettimeofday(&start,NULL);
 
        source >> frame;
        if(frame.empty()) {
            cerr << "frame empty!" << endl; break;
            //'return -1'를 하게되면 다이나믹셀을 비활성화시키는 close함수가 실행되지 않고 코드가 종료되기 때문에
            //다음에 실행을 시키게되면 오류가 날 수 있음.
        }
        if (ctrl_c_pressed) break;

        toUse = frame(Rect(0, frame.rows*3/4, frame.cols, frame.rows/4)); //(1) 영상의 아랫부분 1/4
        cvtColor(toUse, toUse, COLOR_BGR2GRAY); //(2) 그레이스케일영상으로 변환
        toUse += 100 - mean(toUse)[0]; //(3) 영상의 평균밝기를 원하는 값으로 조정
        //결과영상의 픽셀값 = 입력영상의 픽셀값 + (원하는 평균 밝기값 - 입력영상의 평균밝기)
        //mean()의 반환형은 Scalar => 그레이스케일 영상일 경우 첫 번째 원소에 저장됨.
        threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU); //(4) 이진화
        int lable_cnt =  connectedComponentsWithStats(toUse, labels, stats, centroids); //(5) 레이블링
        cvtColor(toUse, toUse, COLOR_GRAY2BGR);
 
        vector<Info> v;
        for(int i=1; i<lable_cnt; i++) { //'0'은 배경
            double *p = centroids.ptr<double>(i); //centroids 행렬의 깊이: CV_64F(double)
            int *q = stats.ptr<int>(i);

            if(q[4] > 100) {
                distance = sqrt(pow((present_point.x - p[0]), 2) + pow((present_point.y - p[1]), 2));
                v.push_back(Info(i, distance));
            }
        }
        sort(v.begin(), v.end(), compare_info); //오름차순
 
        double *p = centroids.ptr<double>(v[0].get_index()); //현재 좌표에서 가장 가까운 객체의 무게중심 좌표
        present_point = Point2d(p[0], p[1]); //현재좌표 초기화
 
        distance = sqrt(pow((present_point.x - past_point.x), 2) + pow((present_point.y - past_point.y), 2));
        //if(distance > toUse.rows / 3) { //카메라가 30fps로 전달하는데 물리적으로 이동이 가능하지 않을 때
        if((abs(present_point.x - past_point.x) > toUse.cols/4) || (abs(present_point.y - past_point.y) > toUse.rows/4)) {
            present_point = past_point;
            //cout << "distance : " << distance << endl;
        }
 
        int error = toUse.cols/2 - present_point.x;
 
        cout << "error: " << error;
        cout << " / Point: " << present_point << endl;
 
        //Blue
        for(int j=1; j<v.size(); j++) {
            double *p = centroids.ptr<double>(v[j].get_index());
            int *q = stats.ptr<int>(v[j].get_index());
            circle(toUse, Point(p[0],p[1]), 3, Scalar(255,0,0), -1);
            rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), Scalar(255,0,0));
        }
        //Red
        circle(toUse, present_point, 3, Scalar(0,0,255), -1);
        //int *q = stats.ptr<int>(v[0].get_index());
        //rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), Scalar(0,0,255));
        
        writer1 << frame;
        writer2 << toUse;
        
        past_point = present_point; //과거좌표 초기화


        usleep(30*1000);
        gettimeofday(&end1,NULL);
        time1 = end1.tv_sec-start.tv_sec + (end1.tv_usec-start.tv_usec)/1000000.0;
        cout << "time:" << time1 << endl;
    }
    return 0;
}

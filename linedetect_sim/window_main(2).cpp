#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void img_adj(VideoCapture& source, Mat& frame, Mat& toUse);
int detection(const Mat& toUse, Mat& stats, Mat& centroids, Point& chase_point, Point& subPoint);
void mark(Mat& toUse, int cnt, Mat& stats, const Mat& centroids, const Point& chase_point);

int main(void) {
    VideoCapture source("7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) { cerr << "Failed to open video file!" << endl; return -1; }

    Mat frame, toUse, stats, centroids;
    Point chase_point(320, 45), subPoint; //chase_point(toUse.cols/2, toUse.rows/2)
    int error;

    while (true)
    {
        img_adj(source, frame, toUse); //사용할 이미지 조정
        if (toUse.empty()) { cerr << "Empty 'toUse' frame in detection or mark!" << endl; break; }

        int cnt = detection(toUse, stats, centroids, chase_point, subPoint);
        mark(toUse, cnt, stats, centroids, chase_point);

        error = (toUse.cols / 2) - chase_point.x;
        cout << "error:" << error << ", time: " << endl;

        //test
        imshow("color image", frame);
        imshow("Object Detection", toUse);

        waitKey(1);
    }
    return 0;
}

//이미지 조정
void img_adj(VideoCapture& source, Mat& frame, Mat& toUse) {
    source >> frame;
    if (frame.empty()) { cerr << "Empty frame encountered!" << endl; return; }

    toUse = frame(Rect(0, frame.rows * 3 / 4, frame.cols, frame.rows / 4)); //(1) 영상의 아랫부분 1/4
    cvtColor(toUse, toUse, COLOR_BGR2GRAY); //(2) 그레이스케일영상으로 변환
    toUse += 100 - mean(toUse)[0]; //(3) 영상의 평균밝기를 원하는 값으로 조정
    //결과영상의 픽셀값 = 입력영상의 픽셀값 + (원하는 평균 밝기값 - 입력영상의 평균밝기)
    //mean()의 반환형은 Scalar => 그레이스케일 영상일 경우 첫 번째 원소에 저장됨.
    threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU); //(4) 이진화
}


int detection(const Mat& toUse, Mat& stats, Mat& centroids, Point& chase_point, Point& subPoint) {
    Mat label;
    int lable_cnt = connectedComponentsWithStats(toUse, label, stats, centroids);

    int min_index = -1; //가장 가까운 객체의 인덱스
    int min_dist = toUse.rows; //최소 거리

    // 객체들을 반복하면서 가장 가까운 객체를 찾기
    for (int i = 1; i < lable_cnt; i++) {
        int area = stats.at<int>(i, 4);  // 객체의 면적(CC_STAT_AREA)
        if (area > 120) {  // 면적이 120보다 큰 객체만 탐지
            Point center(cvRound(centroids.at<double>(i, 0)), cvRound(centroids.at<double>(i, 1)));  // 객체 중심 좌표

            // 객체 중심과 chase_point 간의 거리 계산
            int dist = norm(center - chase_point);
            if (dist < 140 && dist < min_dist) {
                min_dist = dist; min_index = i; // 최소 거리 및 해당 index 갱신
            }
        }
    }

    // 가장 가까운 객체(30ms동안 dist가 140 미만)가 있으면 chase_point로 업데이트
    if (min_index > 0) {
        chase_point = Point(cvRound(centroids.at<double>(min_index, 0)), cvRound(centroids.at<double>(min_index, 1)));
    }
    else {
        //chase_point = subPoint;
        cout << "faaaaaaaaaaaaaaaaaaaaaaaaaaaaar" << endl;
    }
    //subPoint = chase_point;

    return lable_cnt;
}


void mark(Mat& toUse, int cnt, Mat& stats, const Mat& centroids, const Point& chase_point) {
    cvtColor(toUse, toUse, COLOR_GRAY2BGR);  // 컬러영상으로 변환

    for (int i = 1; i < cnt; i++) {
        int area = stats.at<int>(i, 4); //CC_STAT_AREA
        if (area > 120) {
            Point center(cvRound(centroids.at<double>(i, 0)), cvRound(centroids.at<double>(i, 1)));
            Scalar color = (center.x == chase_point.x) ? Scalar(0, 0, 255) : Scalar(255, 0, 0);
            //if (center.x != chase_point.x) { Scalar color = OBJECT_COLOR; }

            //mark
            int* q = stats.ptr<int>(i); //"const int*" 형식의 값을 사용하여 "int*"형식의 엔터티를 초기화할 수 없음 => const(X) Mat& stats
            rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), color, 2);
            circle(toUse, center, 5, color, -1);
        }
    }
    //chase_point
    circle(toUse, chase_point, 5, Scalar(0, 0, 255), -1);
}

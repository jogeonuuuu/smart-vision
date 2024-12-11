//#include <iostream>
#include <unistd.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include "mylib.hpp"

using namespace std;
using namespace cv;

namespace jgw {
    Info::Info(int label_index, double distance, int* q, double* p) {
        this->label_index = label_index; this->distance = distance;
        this->stat_ptr = q; this->centroid_ptr = p;
    }
    int Info::get_index(void) { return this->label_index; }
    double Info::get_distance(void) { return this->distance; }
    int* Info::get_stat_ptr(void) { return this->stat_ptr; }
    double* Info::get_centroid_ptr(void) { return this->centroid_ptr; }


    Mat img_adj(Mat& frame) {
        Mat toUse = frame(Rect(0, frame.rows * 3 / 4, frame.cols, frame.rows / 4)); //(1) 영상의 아랫부분 1/4
        cvtColor(toUse, toUse, COLOR_BGR2GRAY); //(2) 그레이스케일영상으로 변환
        toUse += 100 - mean(toUse)[0]; //(3) 영상의 평균밝기를 원하는 값으로 조정
        //결과영상의 픽셀값 = 입력영상의 픽셀값 + (원하는 평균 밝기값 - 입력영상의 평균밝기)
        //mean()의 반환형은 Scalar => 그레이스케일 영상일 경우 첫 번째 원소에 저장됨.
        threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU); //(4) 이진화

        return toUse;
    }

    Point2d detection(const Mat& toUse, vector<Info> &v, bool& object_ox) {
        Mat labels, stats, centroids;
        int lable_cnt = connectedComponentsWithStats(toUse, labels, stats, centroids);

        static Point2d present_point(320, 45), past_point(320, 45); //현재 및 과거 좌표
        double distance;

        v.clear();
        object_ox = false;

        for(int i=1; i<lable_cnt; i++) { //'0'은 배경
            double *p = centroids.ptr<double>(i); //centroids 행렬의 깊이: CV_64F(double)
            int *q = stats.ptr<int>(i);

            if(q[4] > 100) {
                distance = sqrt(pow((present_point.x - p[0]), 2) + pow((present_point.y - p[1]), 2));
                v.push_back(Info(i, distance, q, p));
            }
        }
        sort(v.begin(), v.end(), compare_info); //오름차순


        if (!v.empty()) { //객체(라인 후보)가 있을 때만
            //double *p = v[0].get_centroid_ptr();
            double *p = centroids.ptr<double>(v[0].get_index()); //현재 좌표에서 가장 가까운 객체의 무게중심 좌표
            present_point = Point2d(p[0], p[1]); //현재좌표 초기화
        }

        //카메라가 30fps로 전달하는데 물리적으로 이동이 가능하지 않을 때
        distance = sqrt(pow((present_point.x - past_point.x), 2) + pow((present_point.y - past_point.y), 2));
        if(distance >  || (abs(present_point.y - past_point.y) > toUse.rows/2)) //toUse.rows / 3
        //if((abs(present_point.x - past_point.x) > toUse.cols/2) || (abs(present_point.y - past_point.y) > toUse.rows/2)) 
        {
            present_point = past_point;
            //cout << "distance : " << distance << endl;

            object_ox = true;
        }

        past_point = present_point; //과거좌표 초기화

        return present_point;
    }

    void mark(Mat& toUse, vector<Info> &v, Point2d& present_point, bool& object_ox) {
        cvtColor(toUse, toUse, COLOR_GRAY2BGR);  // 컬러영상으로 변환

        //Blue
        int start=1;
        if(object_ox) start=0; //따라가는 point를 검출하지 못할 때

        for(int j=start; j<(int)v.size(); j++) {
            double *p = v[j].get_centroid_ptr();
            int *q = v[j].get_stat_ptr();
            circle(toUse, Point(p[0],p[1]), 3, Scalar(255,0,0), -1);
            rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), Scalar(255,0,0));
        }
        
        //Red
        circle(toUse, present_point, 3, Scalar(0,0,255), -1);
    }
}

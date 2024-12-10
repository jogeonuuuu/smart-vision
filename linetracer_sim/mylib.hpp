#ifndef __DETECT_LIB__
#define __DETECT_LIB__

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

namespace jgw {
    class Info { //라인_후보(객체) 정보's
    private:
        int label_index;        //인덱스
        double distance;        //현재값_좌표(present_point)와의 거리
        int* stat_ptr;          //해당 인덱스에 대한 '통계' 정보 : (x,y,width,height,area)
        double* centroid_ptr;   //해당 인덱스에 대한 '무게 중심 좌표' 정보 : (x,y)
    public:
        Info(int label_index, double distance, int* q, double* p);
        int get_index(void);
        double get_distance(void);
        int* get_stat_ptr(void);
        double* get_centroid_ptr(void);
    };
    inline bool compare_info(Info &a, Info &b) { //오름차순
        return a.get_distance() < b.get_distance();
    }

    Mat img_adj(Mat& frame); //이미지 조정
    Point2d detection(const Mat& toUse, vector<Info> &v, bool& object_ox);
    void mark(Mat& toUse, vector<Info> &v, Point2d& present_point, bool& object_ox);
}

#endif //__DETECT_LIB__
#include <iostream>
#include <unistd.h>
#include <signal.h> //'ctrl+c'
#include "opencv2/opencv.hpp"
#include <vector>
#include "mylib.hpp"
#include "dxl.hpp"

using namespace std;
using namespace cv;
using namespace jgw;

bool ctrl_c_pressed = false;
void ctrlc(int) { ctrl_c_pressed = true; }

int main(void)
{
    signal(SIGINT, ctrlc); // SIGINT 신호를 잡아내는 시그널 핸들러 설정

    //string src = "/home/jetson/jogeonuuuu/5_lt_cw_100rpm_out.mp4";
    string src = "/home/jetson/jogeonuuuu/7_lt_ccw_100rpm_in.mp4";
    VideoCapture source(src);
    if (!source.isOpened()) { cout << "Camera error" << endl; return -1; }

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
    if (!writer1.isOpened() && !writer2.isOpened()) { cerr << "Writer open failed!" << endl; return -1; }
 
    //Mat labels, stats, centroids;
 
    //Point2d past_point(320, 45), present_point(320, 45); //과거 및 현재 좌표
    //double distance; //중점(보드으로부터 가장 가까운 객체와의 거리

    struct timeval start, end;
    double time = 0;

    vector<Info> v;
    Mat frame, toUse;
    bool object_ox = true;

    Dxl dxl;
    if(!dxl.open()) { cout << "dynamixel open error"<<endl; return -1; }
    int left_vel = 0, right_vel = 0;
    int left_goal = 0, right_goal = 0;
    bool driving_start = false;

    while(true)
    {
        gettimeofday(&start,NULL);

        source >> frame;
        if (frame.empty()) { cerr << "frame empty!" << endl; break; }
        if (ctrl_c_pressed) break;
        if (dxl.kbhit()) {
            if (dxl.getch() == 's') driving_start = !driving_start;
        }

        toUse = img_adj(frame);
        Point2d line_point = detection(toUse, v, object_ox);
        int error = toUse.cols/2 - (int)line_point.x; //라인 왼쪽   -> 양수
                                                      //라인 오른쪽 -> 음수
        cout << "error: " << error << " / Point: " << line_point << endl;

        //Dynamixel
        left_vel = (200 - (0.32 * error));
        right_vel = -(200 + (0.32 * error));
        if(driving_start) dxl.setVelocity(left_vel, right_vel);

        //Mark
        mark(toUse, v, line_point, object_ox);
        
        writer1 << frame;
        writer2 << toUse;
        
        gettimeofday(&end,NULL);
        time = end.tv_sec-start.tv_sec + (end.tv_usec-start.tv_usec)/1000000.0;
        usleep(30.0*1000 - time); //(30ms - time)동안 대기
        
        cout << "time:" << time  << "s" << endl;
    }
    dxl.close(); // 장치닫기
    return 0;
}
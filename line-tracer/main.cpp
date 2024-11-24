#include <iostream>
#include <signal.h> //'ctrl+c'
#include <opencv2/opencv.hpp>
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

bool compare_info(Info& a, Info& b) { //��������
    return a.get_distance() < b.get_distance();
}


int main(void)
{
    signal(SIGINT, ctrlc); // SIGINT ��ȣ�� ��Ƴ��� �ñ׳� �ڵ鷯 ����

    VideoCapture source("7_lt_ccw_100rpm_in.mp4"); //CAP_FFMPEG
    if (!source.isOpened()) {
        cout << "Video open error" << endl; return -1;
    }

    Mat frame, toUse;
    Mat labels, stats, centroids;

    Point2d past_point(320, 45), present_point(320, 45); //���� �� ���� ��ǥ
    double distance; //����(�������κ��� ���� ����� ��ü���� �Ÿ�

    while (true)
    {
        source >> frame;
        if (frame.empty()) {
            cerr << "frame empty!" << endl; break;
            //'return -1'�� �ϰԵǸ� ���̳��ͼ��� ��Ȱ��ȭ��Ű�� close�Լ��� ������� �ʰ� �ڵ尡 ����Ǳ� ������
            //������ ������ ��Ű�ԵǸ� ������ �� �� ����.
        }
        if (ctrl_c_pressed) break; //"ctrl+c" �Է½�

        toUse = frame(Rect(0, frame.rows * 3 / 4, frame.cols, frame.rows / 4)); //(1) ������ �Ʒ��κ� 1/4
        cvtColor(toUse, toUse, COLOR_BGR2GRAY); //(2) �׷��̽����Ͽ������� ��ȯ
        toUse += 100 - mean(toUse)[0]; //(3) ������ ��չ�⸦ ���ϴ� ������ ����
        //��������� �ȼ��� = �Է¿����� �ȼ��� + (���ϴ� ��� ��Ⱚ - �Է¿����� ��չ��)
        //mean()�� ��ȯ���� Scalar => �׷��̽����� ������ ��� ù ��° ���ҿ� �����.
        threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU); //(4) ����ȭ
        int lable_cnt = connectedComponentsWithStats(toUse, labels, stats, centroids); //(5) ���̺�
        cvtColor(toUse, toUse, COLOR_GRAY2BGR);

        vector<Info> v;
        for (int i = 1; i < lable_cnt; i++) { //'0'�� ���
            double* p = centroids.ptr<double>(i); //centroids ����� ����: CV_64F(double)
            int* q = stats.ptr<int>(i);

            if (q[4] > 100) {
                distance = sqrt(pow((present_point.x - p[0]), 2) + pow((present_point.y - p[1]), 2));
                v.push_back(Info(i, distance));
            }
        }
        sort(v.begin(), v.end(), compare_info); //��������

        double* p = centroids.ptr<double>(v[0].get_index()); //���� ��ǥ���� ���� ����� ��ü�� �����߽� ��ǥ
        present_point = Point2d(p[0], p[1]); //������ǥ �ʱ�ȭ

        distance = sqrt(pow((present_point.x - past_point.x), 2) + pow((present_point.y - past_point.y), 2));
        //if(distance > toUse.rows / 3) { //ī�޶� 30fps�� �����ϴµ� ���������� �̵��� �������� ���� ��
        if ((abs(present_point.x - past_point.x) > toUse.cols/2) || (abs(present_point.y - past_point.y) > toUse.rows/2)) {
            present_point = past_point;
            cout << "distance : " << distance << endl;
        }

        int error = toUse.cols / 2 - present_point.x;

        cout << "error: " << error;
        cout << " / Point: " << present_point << endl;

        //Blue
        for (int j = 1; j < v.size(); j++) {
            double* p1 = centroids.ptr<double>(v[j].get_index());
            int* q1 = stats.ptr<int>(v[j].get_index());
            circle(toUse, Point(p1[0], p1[1]), 3, Scalar(255, 0, 0), -1);
            rectangle(toUse, Rect(q1[0], q1[1], q1[2], q1[3]), Scalar(255, 0, 0));
        }
        //Red
        circle(toUse, present_point, 3, Scalar(0, 0, 255), -1);
        //int *q = stats.ptr<int>(v[0].get_index());
        //rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), Scalar(0,0,255));

        past_point = present_point; //������ǥ �ʱ�ȭ

        imshow("����", frame);
        imshow("toUse", toUse);

        waitKey(1); //usleep(30 * 1000);
    }
    return 0;
}
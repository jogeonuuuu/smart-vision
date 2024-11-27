#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void img_adj(VideoCapture& source, Mat& frame, Mat& toUse);
void detection(const Mat& toUse, Mat& stats, Mat& centroids, Point& chase_point, Point& subPoint);
void mark(const Mat& toUse, Mat& stats, const Mat& centroids, const Point& chase_point);

int main(void) {
    VideoCapture source("7_lt_ccw_100rpm_in.mp4");
    if (!source.isOpened()) { cerr << "Failed to open video file!" << endl; return -1; }

    Mat frame, toUse, stats, centroids;
    Point chase_point(320, 45), subPoint; //chase_point(toUse.cols/2, toUse.rows/2)
    int error;

    while (true)
    {
        img_adj(source, frame, toUse); //����� �̹��� ����
        if (toUse.empty()) { cerr << "Empty 'toUse' frame in detection or mark!" << endl; break; }

        detection(toUse, stats, centroids, chase_point, subPoint);
        mark(toUse, stats, centroids, chase_point);

        error = (toUse.cols / 2) - chase_point.x;
        cout << "error:" << error << ", time: " << endl;

        //test
        imshow("color image", frame);
        imshow("Object Detection", toUse);

        waitKey(1);
    }
    return 0;
}

//�̹��� ����
void img_adj(VideoCapture& source, Mat& frame, Mat& toUse) {
    source >> frame;
    if (frame.empty()) { cerr << "Empty frame encountered!" << endl; return; }

    toUse = frame(Rect(0, frame.rows * 3 / 4, frame.cols, frame.rows / 4)); //(1) ������ �Ʒ��κ� 1/4
    cvtColor(toUse, toUse, COLOR_BGR2GRAY); //(2) �׷��̽����Ͽ������� ��ȯ
    toUse += 100 - mean(toUse)[0]; //(3) ������ ��չ�⸦ ���ϴ� ������ ����
    //��������� �ȼ��� = �Է¿����� �ȼ��� + (���ϴ� ��� ��Ⱚ - �Է¿����� ��չ��)
    //mean()�� ��ȯ���� Scalar => �׷��̽����� ������ ��� ù ��° ���ҿ� �����.
    threshold(toUse, toUse, 0, 255, THRESH_BINARY | THRESH_OTSU); //(4) ����ȭ
}


void detection(const Mat& toUse, Mat& stats, Mat& centroids, Point& chase_point, Point& subPoint) {
    Mat label;
    int lable_cnt = connectedComponentsWithStats(toUse, label, stats, centroids);
    cvtColor(toUse, toUse, COLOR_GRAY2BGR);  // �̹����� �÷��� ��ȯ

    int min_index = -1; //���� ����� ��ü�� �ε���
    int min_dist = toUse.rows; //�ּ� �Ÿ�

    // ��ü���� �ݺ��ϸ鼭 ���� ����� ��ü�� ã��
    for (int i = 1; i < lable_cnt; i++) {
        int area = stats.at<int>(i, CC_STAT_AREA);  // ��ü�� ����(4)
        if (area > 120) {  // ������ 120���� ū ��ü�� Ž��
            Point center(cvRound(centroids.at<double>(i, 0)), cvRound(centroids.at<double>(i, 1)));  // ��ü �߽� ��ǥ

            // ��ü �߽ɰ� chase_point ���� �Ÿ� ���
            int dist = norm(center - chase_point);
            if (dist < 140 && dist < min_dist) {
                min_dist = dist; min_index = i; // �ּ� �Ÿ� �� �ش� index ����
            }
        }
    }

    // ���� ����� ��ü(30ms���� dist�� 140 �̸�)�� ������ chase_point�� ������Ʈ
    if (min_index > 0) {
        chase_point = Point(cvRound(centroids.at<double>(min_index, 0)), cvRound(centroids.at<double>(min_index, 1)));
    }
    else {
        //chase_point = subPoint;
        cout << "faaaaaaaaaaaaaaaaaaaaaaaaaaaaar" << endl;
    }
    //subPoint = chase_point;
}


void mark(const Mat& toUse, Mat& stats, const Mat& centroids, const Point& chase_point) {
    for (int i = 1; i < stats.rows; i++) {
        // ������ ������ ������ ū ��ü�� ǥ��
        int area = stats.at<int>(i, 4); //CC_STAT_AREA
        if (area > 120) {
            Point center(cvRound(centroids.at<double>(i, 0)), cvRound(centroids.at<double>(i, 1)));
            Scalar color = (center.x == chase_point.x) ? Scalar(0,0,255) : Scalar(255,0,0);
            //if (center.x != chase_point.x) { Scalar color = OBJECT_COLOR; }

            //mark
            int* q = stats.ptr<int>(i); //"const int*" ������ ���� ����Ͽ� "int*"������ ����Ƽ�� �ʱ�ȭ�� �� ���� => const(X) Mat& stats
            rectangle(toUse, Rect(q[0], q[1], q[2], q[3]), color, 2);
            circle(toUse, center, 5, color, -1);
        }
    }
    //chase_point
    circle(toUse, chase_point, 5, Scalar(0,0,255), -1);
}
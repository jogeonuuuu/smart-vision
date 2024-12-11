[시연 동영상_inline](https://www.youtube.com/watch?v=tSsMGaWjzh0)

[시연 동영상_outline](https://www.youtube.com/watch?v=yzUuTJRe2NU)


<br/><br/><br/><br/>


[Makefile 설명](https://github.com/jogeonuuuu/smart-vision/blob/main/linedetect_sim/README.md)

</p>


#### //mylib.cpp



###Info 클래스
생성자: Info(int label_index, double distance, int* q, double* p)
객체의 라벨 인덱스, 현재 좌표와 객체 중심 간의 거리, 통계 및 중심 좌표 데이터를 초기화
멤버 함수:
get_index(): 라벨 인덱스를 반환
get_distance(): 중심 좌표와의 거리를 반환
get_stat_ptr(): 객체의 통계 데이터를 가리키는 포인터를 반환
get_centroid_ptr(): 객체의 중심 좌표 데이터를 가리키는 포인터를 반환

</p>

###Mat img_adj(Mat& frame)
입력된 영상에서 특정 부분(아랫부분 1/4)을 추출하여 처리
처리 단계:
추출된 영역을 그레이스케일 영상으로 변환
평균 밝기를 조정하여 원하는 밝기 수준으로 보정
Otsu 알고리즘을 사용해 이진화 처리
반환값: 처리된 이진화 영상

</p>

###Point2d detection(const Mat& toUse, vector<Info>& v, bool& object_ox)
이진화된 영상을 기반으로 객체를 탐지
주요 동작:
연결된 컴포넌트를 분석하여 객체 정보를 추출.
중심 좌표와의 거리 및 객체의 통계 데이터를 저장.
객체 리스트를 거리 기준으로 정렬 후 가장 가까운 객체의 중심 좌표를 반환.
현재 좌표와 이전 좌표 간 거리를 비교하여 객체 유무를 결정.
입력값:
toUse: 이진화된 입력 영상.
v: 객체 정보를 저장하는 벡터.
object_ox: 객체 감지 상태를 저장하는 변수.
반환값: 현재 탐지된 객체의 좌표.

</p>


void mark(Mat& toUse, vector<Info>& v, Point2d& present_point, bool& object_ox)
탐지된 객체를 영상에 표시
주요 동작:
입력 이진화 영상을 컬러 영상으로 변환
객체들을 직사각형과 원으로 시각화
파란색으로 일반 객체 표시
빨간색으로 탐지된 현재 좌표 표시
객체가 감지되지 않을 경우 object_ox 값을 기반으로 적절한 표시를 수행

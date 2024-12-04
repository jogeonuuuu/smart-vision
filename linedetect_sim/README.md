


<p align="center">
<img src="https://github.com/user-attachments/assets/d17dae01-d5d2-4db6-825f-51a3d8c3dcd9" width="50%" height="50%"/>
<img src="https://github.com/user-attachments/assets/dd1e9562-f0d3-4ee1-aaec-344e9cec44eb" width="50%" height="50%"/>

</p>


#### //mylib.hpp

##### <line 15>
Mat 객체인 stats의 label_index 행에 대한 주소.
해당 행(label_index)에는 label_index 대한 **통계 정보** (x,y,width,height,area)가 들어있음.
(stat_ptr[0] ~ stat_ptr[4])

<br/>

##### <line 16>
Mat 객체인 centroids의 label_index 행에 대한 주소.
해당 행(label_index)에는 label_index 대한 **무계 중심 좌표 정보** (x,y)가 들어있음.
(centroid_ptr[0] ~ centroid_ptr[1])

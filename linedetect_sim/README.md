[시연 동영상_inline](https://www.youtube.com/watch?v=tSsMGaWjzh0)

[시연 동영상_outline](https://www.youtube.com/watch?v=yzUuTJRe2NU)


<br/><br/><br/><br/>


[Makefile 설명](https://github.com/jogeonuuuu/smart-vision/blob/main/linedetect_sim/README.md)

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

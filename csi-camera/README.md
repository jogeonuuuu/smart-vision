
gstreamer를 통한 실시간 스트리밍
===
<img src="https://github.com/user-attachments/assets/ec045313-7ef8-4cac-94df-2187947d34fb" width="60%" height="60%"/>

```
(1) csi camera(IMX219) -> jetson nano
(2) jetson nano -> window
```


<br/><br/><br/><br/>


#### (1) csi camera(IMX219) -> jetson nano
<img src="https://github.com/user-attachments/assets/bf35c362-22b5-477c-82ce-e7f7bc17ce24" width="50%" height="50%"/>
https://github.com/jogeonuuuu/smart-vision/blob/ae718efa64e6b0df40854ad1e9748df9152b499e/main.cpp#L7-L13

```
영상 처리를 하기 위해 카메라와의 연결 코드 + 옵션(csi camera는 gstreamer 패키지 사용)
- 영상 출력 속도 : 30fps (framerate=(fraction)30/1)
- 영상 크기 : 640 x 360
- 포맷 : BGR
```
 
#### (2) jetson nano -> window
<img src="https://github.com/user-attachments/assets/735c14a5-b10f-4cfe-9d02-5f00214f1a20" width="50%" height="50%"/>
https://github.com/jogeonuuuu/smart-vision/blob/bdc44ccfae6465a418ca334eea5b9cd1730184d4/csi-camera/main.cpp#L16-L20
 
```
네트워크를 통해 전송하기 때문에 다음과 같은 옵션을 줌
- 전송할 ip주소: 203.234.XX.XXX
- 포트번호: XXXX
```


<br/><br/><br/><br/>


### 결과영상
- Jetson board에서 Gstreamer Server 실행
- windows의 powershell에서 gstreamer client 실행

```
실시간으로 받아온 이미지를 처리 후 제어하는데 **초당 30번. 즉, 한 번당 33ms** 이상이 되면 안 됨
```
 
![image](https://github.com/user-attachments/assets/4f8d13a5-752c-4144-a012-e4f1265f495a)
<img src="https://github.com/user-attachments/assets/b182f596-a090-4c8f-bc7d-4d5fd93120fc" width="70%" height="70%"/>


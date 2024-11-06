

### gstreamer
![image](https://github.com/user-attachments/assets/ec045313-7ef8-4cac-94df-2187947d34fb)

 <img src="https://github.com/user-attachments/assets/bf35c362-22b5-477c-82ce-e7f7bc17ce24" width="50%" height="50%"/>
https://github.com/jogeonuuuu/smart-vision/blob/ae718efa64e6b0df40854ad1e9748df9152b499e/main.cpp#L7-L13

```
'IMX219 csi camera -> windows 환경'으로 영상을 가져오는 코드 + 옵션  (csi camera => gstreamer 패키지 사용)
- 영상 출력 속도 : 30fps (framerate=(fraction)30/1)
- 영상 크기 : 640 x 360
- 포맷 : BGR
```

 <img src="https://github.com/user-attachments/assets/735c14a5-b10f-4cfe-9d02-5f00214f1a20" width="50%" height="50%"/>
https://github.com/jogeonuuuu/smart-vision/blob/ae718efa64e6b0df40854ad1e9748df9152b499e/main.cpp#L16-L20

```
네트워크를 통해 전송하기 때문에 다음과 같은 옵션을 줌.
- 전송할 ip주소: 203.234.58.155
- 포트번호: XXXX
```



### 결과영상
- Jetson board에서 Gstreamer Server 실행
- windows의 powershell에서 gstreamer client 실행

![image](https://github.com/user-attachments/assets/4f8d13a5-752c-4144-a012-e4f1265f495a)
<img src="https://github.com/user-attachments/assets/b182f596-a090-4c8f-bc7d-4d5fd93120fc" width="70%" height="70%"/>

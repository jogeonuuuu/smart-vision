[시연 동영상_inline](https://www.youtube.com/watch?v=tSsMGaWjzh0)

[시연 동영상_outline](https://www.youtube.com/watch?v=yzUuTJRe2NU)


<br/><br/><br/><br/>


소스코드
----
 https://github.com/jogeonuuuu/smart-vision/blob/761abaf27ed51d5811106142b9d987c2244b2ec9/camera-and-dynamixel/cam_act/main.cpp#L61-L66
 ```
 다음 코드는 가감속과 등속을 수행
 => 현재의 모터 속도가 입력한 속도까지 점차적으로 가속 또는 감속을 수행하여 진동 및 소음을 최소화
 ```


<br/><br/><br/><br/>


Makefile
----
<p align="center">
<img src="https://github.com/user-attachments/assets/2630bf1a-b455-4bde-912d-f4c02d6b84d6" width="50%" height="50%"/>
<img src="https://github.com/user-attachments/assets/1fbce266-5b28-4726-8d9e-787ea6671a99" width="50%" height="50%"/>
</p>


https://github.com/jogeonuuuu/smart-vision/blob/4441b05cd43aafd5ccb6ef5e6a42592b6376f7b9/cam-act/Makefile#L1-L18
- **Line 3** & **Line 4** :
  - **표준 라이브러리**가 아닌 **사용자 라이브러리**를 사용하기 위해서는 GCC 명령어 설정이 필요
    ```
    <GCC 명령어 옵션>
    
    - I<header_dir> : 사용자 라이브러리의 헤더파일 경로 지정 (선행처리 단계)
    - l<lib_name> : 라이브러리 (목적)파일
    - L<lib_dir(path)> : 라이브러리 (목적)파일 경로
                         동적라이브러리(so) 경로는 라이브러리 설치시 /usr/lib 아래에 설치됨
                         운영체제 설정파일(/etc/ld.so.conf)에 경로가 저장되어 설정이 필요없음
                         -lrt 옵션 : 실시간 기능을 제공하는 'librt' 라이브러리를 링크하는 옵션 
    ```
    [시스템 설정 파일(/etc/ld.so.conf)](https://blog.naver.com/demonic3540/222396863191)

  - **pkg-config :** 헤더파일경로, 라이브러리 파일을 자동으로 찾아주는 명령어
    ```
    - 설치된 라이브러리 목록 : --list-all
    - 헤더파일 경로 : --cflags
    - 라이브러리 파일 : --libs
    ```
  - 컴파일러가 헤더파일 경로, 라이브러리 목적파일 및 경로를 모르기 때문에 직접 설정을 해주는 것

<br/>

- **Line 9~10** : 
  - 실행파일(TARGET)을 만드는 규칙(Rule) -> **링크 규칙**
  - Makefile의 첫번째 Rule이 실행파일을 생성하는 것이어야 함
  - 실행파일을 만들기위한 목적파일이 없을 때 컴파일 규칙이 먼저 실행됨

<br/>

- **Line 11~14** : 목적파일(OBJS)을 만드는 규칙(Rule) -> **컴파일 규칙**

<br/>

- **Line 16~18** : 'clean'은 파일이 아님. target을 만들기위한 prerequisites(필요항목)이 되는 오류를 방지하기위해 명시


<br/><br/><br/><br/>




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

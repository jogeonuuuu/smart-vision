소스코드
----
 https://github.com/jogeonuuuu/smart-vision/blob/761abaf27ed51d5811106142b9d987c2244b2ec9/camera-and-dynamixel/cam_act/main.cpp#L61-L66
 ```
 다음 코드는 가감속과 등속을 수행
 => 현재의 모터 속도가 입력한 속도까지 점차적으로 가속 또는 감속을 수행하여 진동 및 소음을 최소화
 ```


<br/><br/><br/><br/><br/>


Makefile
----
<p align="center">
<img src="https://github.com/user-attachments/assets/d6599164-92c1-414a-b820-04db1a2fdfec" width="50%" height="50%"/>
<img src="https://github.com/user-attachments/assets/1fbce266-5b28-4726-8d9e-787ea6671a99" width="50%" height="50%"/>
</p>


https://github.com/jogeonuuuu/smart-vision/blob/4441b05cd43aafd5ccb6ef5e6a42592b6376f7b9/cam-act/Makefile#L1-L18
- **Line 3** & **Line 4** :
  - **표준 라이브러리**가 아닌 **사용자 라이브러리**를 사용하기 위해서는 GCC 명령어 설정이 필요
    ```
    <GCC 명령어 옵션>
    
    - I<header_dir> : 사용자 라이브러리의 헤더파일 경로 지정 (선행처리 단계)
    - l<lib_name> : (정적)라이브러리 파일(옵션)
    - L<lib_dir(path)> : (정적)라이브러리 파일 경로, 동적라이브러리(so) 경로는 라이브러리 설치시(/usr/lib 아래에 설치됨) 운영체제 설정파일에 저장되어 설정이 필요없음(/etc/ld.so.conf)
    ```
  - 컴파일러가 헤더파일 경로, 라이브러리 목적파일 및 경로를 모르기 때문에 직접 설정을 해주는 것

  


- **Line 9~10** : 
  - 실행파일(TARGET)을 만드는 규칙(Rule) -> **링크 규칙**
  - Makefile의 첫번째 Rule이 실행파일을 생성하는 것이어야 함
  - 실행파일을 만들기위한 목적파일이 없을 때 컴파일 규칙이 먼저 실행됨

- **Line 11~14** : 목적파일(OBJS)을 만드는 규칙(Rule) -> **컴파일 규칙**
- **Line 16~18** : 'clean'은 파일이 아님. target을 만들기위한 prerequisites(필요항목)이 되는 오류를 방지하기위해 명시


<br/><br/><br/><br/><br/>


실행결과
----
https://github.com/user-attachments/assets/87723293-bbed-4f0b-8059-282dafb4a0aa

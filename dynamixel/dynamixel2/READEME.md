### 1. 정지명령(' '입력)을 입력했을 때와 프로그램을 종료했을 때(ctrl+c) 바퀴를 손으로 움직일 때 느껴지는 토크 차이를 설명하라.
정지 명령을 입력했을 때 >> 'mx.setVelocity(0,0)'와 같이 모터가 정지할 수 있게 제어를 하고있는 것이다.
프로그램을 종료했을 때(Ctrl+c) >> 시그널 핸들러를 아래 코드와 같이 설정하였으므로 'Ctrl+c' 입력시 강제종료가 아닌 while문을 탈출한 후에 정상적으로 다이나믹셀을 종료한다. 
이에 따라 바퀴를 손으로 움직일 때 토크값이 느껴지지 않는 것이다.
https://github.com/jogeonuuuu/smart-vision/blob/09df034b4929ab381e5ac4796fe64a8f3e50a81d/dynamixel/dynamixel2/main.cpp#L7-L8
https://github.com/jogeonuuuu/smart-vision/blob/09df034b4929ab381e5ac4796fe64a8f3e50a81d/dynamixel/dynamixel2/main.cpp#L34

### 2. Ctrl+c를 눌렀을때 실습과제1의 결과와 다른 이유를 설명하라.
실습과제 1번과는 다르게 'Ctrl+c'를 입력하면 리눅스에서 프로세스에 SIGINT라는 시그널을 전달하고, 추가한 시그널 핸들러(signal handler)를 실행시키기 때문에 바로 종료가 되는 것을 확인할 수 있다.

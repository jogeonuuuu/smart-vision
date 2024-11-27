### - 소스코드
> https://github.com/jogeonuuuu/smart-vision/blob/761abaf27ed51d5811106142b9d987c2244b2ec9/camera-and-dynamixel/cam_act/main.cpp#L61-L66
> 다음 코드는 가감속과 등속을 수행
> 
> => 현재의 모터 속도가 입력한 속도까지 점차적으로 가속 또는 감속을 수행하여 진동 및 소음을 최소화



### - Makefile
https://github.com/jogeonuuuu/smart-vision/blob/74a0afdc981359569e7ffd313f12b5169a2edb79/cam-act/Makefile#L1-L16
> - Line 3 : 
>
> - Line 4 : 
> 
> - Line 7~8 : 
>   - 실행파일(TARGET)을 만드는 규칙(Rule) -> 링크 규칙
>   - Makefile의 첫번째 Rule이 실행파일을 생성하는 것이어야 함
>   - 실행파일을 만들기위한 목적파일이 없을 때 컴파일 규칙이 먼저 실행됨
> 
> - Line 9~12 : 목적파일(OBJS)을 만드는 규칙(Rule) -> 컴파일 규칙
>
> - Line 14~16 : 'clean'은 파일이 아님. target을 만들기위한 prerequisites(필요항목)이 되는 오류를 방지하기위해 명시



### - 실행결과
https://github.com/user-attachments/assets/87723293-bbed-4f0b-8059-282dafb4a0aa

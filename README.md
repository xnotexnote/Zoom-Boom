# Zoom-Boom
Winapi로 만들어진 프로그램의 화면공유 방지 프로그램   
## 사용법
DLL폴더의 소스코드를 dll로 컴파일한 후 winmain.cpp의 DLL_NAME을 dll의 위치로 설정한 뒤 컴파일 하셔서 실행시키시면 됩니다.
## 구현
콘솔 환경에서 DLL 인젝션을 통해, 각 프로세스에서 SetWindowDisplayAffinity 함수를 사용하여 해당 프로세스가 소유한 모든 최상위 윈도우들의 WindowDisplayAffinity 설정을 0x11로 변경합니다.   
ms docs에 의하면, The window is displayed only on a monitor. Everywhere else, the window does not appear at all.   
One use for this affinity is for windows that show video recording controls, so that the controls are not included in the capture.   
Introduced in Windows 10 Version 2004. See remarks about compatibility regarding previous versions of Windows.   
즉, 해당 윈도우는 모니터에만 표시되게 됩니다.
## 주의사항
모든 프로그램이 다 적용되지 않을 수 있습니다.   
64비트 윈도우즈 환경에서 32비트로 컴파일하여 테스트되었습니다.

# Zoom-Boom
Winapi로 DLL인젝션과 SetWindowDisplayAffinity함수로 다른 프로그램에 의한 프로그램의 화면공유 방지 프로그램   
## 사용법
DLL폴더의 소스코드를 dll로 컴파일한 후 winmain.cpp의 DLL_NAME을 dll의 위치로 설정한 뒤 컴파일 하셔서 실행시키시면 됩니다.
## 주의사항
모든 프로그램이 다 적용되지 않을 수 있습니다.   
64비트 윈도우즈 환경에서 32비트로 컴파일하여 테스트되었습니다.

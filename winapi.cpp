// winapi.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#define DLL_NAME ("C:\\Users\\xnote\\source\\repos\\Dll1\\Release\\Dll1.dll")
int main()
{
	setlocale(LC_ALL, "KOREAN");
	OutputDebugStringA("Winapi!");
	printf("Zoom, Boom!\n");
	DWORD processIds[1024];
	DWORD bytes;
	if (EnumProcesses(processIds, sizeof processIds, &bytes) != 0) {
		printf("Process count: %ld\n", bytes / sizeof DWORD);
		for (int i = 0; i < bytes / sizeof DWORD; i++) {
			HANDLE handle;
			printf("Open Process\n");
			handle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processIds[i]);
			if (handle != 0) {
				char name[256];
				DWORD size = 256;
				if (QueryFullProcessImageNameA(handle, 0, name, &size) != 0) {
					printf("Process name: %s\n", name);
				}
				LPVOID remoteBuf = NULL;
				printf("Virtual alloc\n");
				remoteBuf = VirtualAllocEx(handle, NULL, strlen(DLL_NAME)+(SIZE_T)1, MEM_COMMIT, PAGE_READWRITE);
				if (remoteBuf != nullptr) {
					printf("Write\n");
					WriteProcessMemory(handle, remoteBuf, DLL_NAME, strlen(DLL_NAME)+ (SIZE_T)1, NULL);
					HMODULE hM;
					printf("Get module handle\n");
					hM = GetModuleHandle(L"kernel32.dll");
					if (hM != 0) {
						LPTHREAD_START_ROUTINE thread;
						printf("Get Proc Address LoadLibraryA\n");
						
						thread = (LPTHREAD_START_ROUTINE)GetProcAddress(hM, "LoadLibraryA");
						printf("CreateRemoteThread\n");
						HANDLE remoteThread = CreateRemoteThread(handle, NULL, 0, thread, remoteBuf, 0, NULL);
						if (remoteThread != 0) {
							printf("Wait\n");
							WaitForSingleObject(remoteThread, 1000);
							printf("Close\n");
							CloseHandle(handle);
							CloseHandle(remoteThread);
						}
						else {
							processIds[i] = GetLastError();
							printf("Error_code: %ld", processIds[i]);
							TCHAR* message = nullptr;
							FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, processIds[i], MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
							wprintf(L"Error: %s\n", message);
							LocalFree(message);
							VirtualFreeEx(handle, remoteBuf, 0, MEM_RELEASE);
							CloseHandle(handle);
						}
					}
					else {
						processIds[i] = GetLastError();
						printf("Error_code: %ld", processIds[i]);
						TCHAR* message = nullptr;
						FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, processIds[i], MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
						wprintf(L"Error: %s\n", message);
						LocalFree(message);
						VirtualFreeEx(handle, remoteBuf, 0, MEM_RELEASE);
						CloseHandle(handle);
					}
				}
				else {
					processIds[i] = GetLastError();
					printf("Error_code: %ld", processIds[i]);
					TCHAR* message = nullptr;
					FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, processIds[i], MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
					wprintf(L"Error: %s\n", message);
					LocalFree(message);
					
					CloseHandle(handle);
				}
			}
			else {
				processIds[i] = GetLastError();
				printf("Error_code: %ld", processIds[i]);
				TCHAR* message = nullptr;
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, processIds[i], MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
				wprintf(L"Error: %s\n", message);
				LocalFree(message);
			}
//			EnumThreadWindows(processIds[i], callback, (LPARAM)NULL);
		}
	}
	else {
		DWORD errorN = GetLastError();
		printf("Error_code: %ld", errorN);
		TCHAR* message = nullptr;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, errorN, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
		wprintf(L"Error: %s\n", message);
		LocalFree(message);
		return -1;
	}
	return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

#include <Windows.h>
#include <string>
#include <functional>
DWORD* pI;
BOOL CALLBACK EnumThreadWindowsCallback(HWND hW, LPARAM lP) {
    DWORD myPI;
    GetWindowThreadProcessId(hW, &myPI);
    if (myPI != 0) {
        if (myPI == *pI) {
            if (SetWindowDisplayAffinity(hW, 0x11) == FALSE) {
                DWORD result = GetLastError();
                OutputDebugStringA(("Error_code: " + std::to_string(result)).c_str());
                TCHAR* message = nullptr;
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (TCHAR*)&message, 0, nullptr);
                OutputDebugStringW(((std::wstring)L"Error: " + message).c_str());
                LocalFree(message);
            }
        }
    }
    return TRUE;
}
DWORD WINAPI callback(LPVOID lP) {
    DWORD pI = GetCurrentProcessId();
    ::pI = &pI;
    if(pI!=0)
        EnumWindows(EnumThreadWindowsCallback, (LPARAM)NULL);
    return 0;
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HANDLE thread = CreateThread(NULL, 0, callback, NULL, 0, NULL);
        if (thread != 0) {
            WaitForSingleObject(thread, 10000);
            CloseHandle(thread);
        }
        break;
    }
    return TRUE;
}


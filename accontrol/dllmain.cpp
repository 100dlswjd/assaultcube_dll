// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"

HMODULE g_hMod = NULL;

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    BYTE hp = 1000;
    DWORD baseAddr = 0x400000;
    DWORD ptrAddr;
    DWORD hpAddr = baseAddr + 0x0017E0A8;
    DWORD hp_offset = 0xEC;
    ReadProcessMemory(g_hMod, (void*)hpAddr, &ptrAddr, sizeof(DWORD), 0);
    WriteProcessMemory(g_hMod, (void*)(ptrAddr + hp_offset), (void*)hp, 5, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE hThread = NULL;
    g_hMod = (HMODULE)hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        OutputDebugString(L"Injection success !!");
        hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
        CloseHandle(hThread);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


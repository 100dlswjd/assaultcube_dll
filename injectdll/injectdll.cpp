#include"windows.h"
#include"tchar.h"

BOOL InjectDLL(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	// 1.
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed..! [%d]\n", dwPID, GetLastError());
		return FALSE;
	}
	// 2.
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// 3.
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

	// 4.
	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

	// 5.
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 3)
	{
		_tprintf(L"USAGE : %s pid dll_path\n", argv[0]);
		return 1;
	}

	// inject_dll
	if (InjectDLL((DWORD)_tstol(argv[1]), argv[2]))
		_tprintf(L"InjectDLL (\"%s\") success !!! \n", argv[2]);
	else
		_tprintf(L"InjectDLL (\"%s\") failed.. ! \n", argv[2]);

	return 0;
}
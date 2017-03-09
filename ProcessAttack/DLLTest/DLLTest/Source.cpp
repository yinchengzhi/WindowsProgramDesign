#include<Windows.h>
#include<tchar.h>
BOOL WINAPI DLLMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		MessageBox(NULL, _T("病源体已成功寄生目标进程"), _T("信息"), MB_ICONINFORMATION);
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		MessageBox(NULL, _T("病源体已成功脱离目标进程"), _T("信息"), MB_ICONINFORMATION);
	}
	break;
	}
	return TRUE;
}
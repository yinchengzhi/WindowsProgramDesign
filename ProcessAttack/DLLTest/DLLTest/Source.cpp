#include<Windows.h>
#include<tchar.h>
BOOL WINAPI DLLMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		MessageBox(NULL, _T("��Դ���ѳɹ�����Ŀ�����"), _T("��Ϣ"), MB_ICONINFORMATION);
	}
	break;
	case DLL_PROCESS_DETACH:
	{
		MessageBox(NULL, _T("��Դ���ѳɹ�����Ŀ�����"), _T("��Ϣ"), MB_ICONINFORMATION);
	}
	break;
	}
	return TRUE;
}
#include<Windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nCmdShow)
{
	//HWND hwnd = FindWindowA(NULL, "SSH�����ֲ�.txt - ���±�");
	HWND hwnd = FindWindowA("Chrome_WidgetWin_1", NULL);
	int nRet = MessageBoxA(hwnd, "�����ҵĵ�һ��Windows����", "лл", MB_YESNOCANCEL | MB_ICONWARNING);
	switch (nRet)
	{
	case IDYES:
		MessageBoxA(NULL, "��������yes", "���:", MB_OK);
		break;
	case IDNO:
		MessageBoxA(NULL, "��������no", "���:", MB_OK);
		break;
	case IDCANCEL:
		MessageBoxA(NULL, "��������cancel", "���:", MB_OK);
		break;
	}
	return 0;
}
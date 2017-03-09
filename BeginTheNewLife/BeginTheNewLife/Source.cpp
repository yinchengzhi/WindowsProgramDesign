#include<Windows.h>
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nCmdShow)
{
	//HWND hwnd = FindWindowA(NULL, "SSH连接手册.txt - 记事本");
	HWND hwnd = FindWindowA("Chrome_WidgetWin_1", NULL);
	int nRet = MessageBoxA(hwnd, "这是我的第一个Windows程序", "谢谢", MB_YESNOCANCEL | MB_ICONWARNING);
	switch (nRet)
	{
	case IDYES:
		MessageBoxA(NULL, "你点击的是yes", "结果:", MB_OK);
		break;
	case IDNO:
		MessageBoxA(NULL, "你点击的是no", "结果:", MB_OK);
		break;
	case IDCANCEL:
		MessageBoxA(NULL, "你点击的是cancel", "结果:", MB_OK);
		break;
	}
	return 0;
}
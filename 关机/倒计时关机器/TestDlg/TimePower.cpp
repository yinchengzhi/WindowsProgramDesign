
#include"TimePower.h"

int nHour = 0;
int nMinute = 0;
int nSecond = 0;
LPCTSTR szAppName = TEXT("逼哥吃翔！");
int nH = 0, nM = 0, nS = 0;										//存储小时，分钟，秒数

void DisplayDigit(HDC hdc, int iNumber)
{
	static BOOL fSevenSegment[10][7] = {
		1, 1, 1, 0, 1, 1, 1,     // 0  
		0, 0, 1, 0, 0, 1, 0,     // 1  
		1, 0, 1, 1, 1, 0, 1,     // 2  
		1, 0, 1, 1, 0, 1, 1,     // 3  
		0, 1, 1, 1, 0, 1, 0,     // 4  
		1, 1, 0, 1, 0, 1, 1,     // 5  
		1, 1, 0, 1, 1, 1, 1,     // 6  
		1, 0, 1, 0, 0, 1, 0,     // 7  
		1, 1, 1, 1, 1, 1, 1,     // 8  
		1, 1, 1, 1, 0, 1, 1      // 9  
	};

	static POINT ptSegment[7][6] = {
		7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
		6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
		36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
		7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
		6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
		36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
		7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70
	};

	int          iSeg;

	for (iSeg = 0; iSeg < 7; iSeg++)
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6);
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
	//if (!fSuppress || (iNumber / 10 != 0))  
	DisplayDigit(hdc, iNumber / 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);

	DisplayDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)
{
	POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
		2,  51,  6,  47,  10, 51,  6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st;

	GetLocalTime(&st);
	if (nH == 0) 
		nH = nHour - st.wHour;

	if (st.wMinute >= nMinute && nH > 0)							//感觉这个地方有每个小时多算一秒的误差.....
	{
		nH -= 1;
		nM = 60 + nMinute - st.wMinute;
	}
	else
		nM = nMinute - st.wMinute;

	if (st.wSecond > nSecond && nM > 0)
	{
		nM -= 1;
		nS = 60 + nSecond - st.wSecond;
	}
	else
		nS = nSecond - st.wSecond;

	if (nH == 0 && nM == 0 && nS == 0)
	{
		system("shutdown -s -t 60");
		exit(1);
	}

	//if (f24Hour)
	DisplayTwoDigits(hdc, nH, fSuppress);
	//else
	//	DisplayTwoDigits(hdc, (nH %= 12) ? nH : 12, fSuppress);

	DisplayColon(hdc);
	DisplayTwoDigits(hdc, nM, fSuppress);
	DisplayColon(hdc);
	DisplayTwoDigits(hdc, nS, fSuppress);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL     f24Hour, fSuppress;
	static HBRUSH   hBrushRed;
	static int      cxClient, cyClient;
	HDC             hdc;
	PAINTSTRUCT     ps;
	TCHAR           szBuffer[2];
	HINSTANCE hInstance = NULL;												//最小化
	NOTIFYICONDATA nti;														//最小化
	UINT WM_TASKBARCREATED;


	// 不要修改TaskbarCreated，这是系统任务栏自定义的消息
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));

	switch (message)
	{
	case WM_CREATE:
		hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
		SetTimer(hwnd, ID_TIMER, 1000, NULL);
		nti.cbSize = sizeof(nti);
		nti.hWnd = hwnd;
		nti.uID = 0;
		nti.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nti.uCallbackMessage = WM_USER;
		nti.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		lstrcpy(nti.szTip, szAppName);
		Shell_NotifyIcon(NIM_ADD, &nti);
		// fall through  

	case WM_USER:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			ShowWindow(hwnd, SW_SHOWNORMAL);
			break;
		case WM_RBUTTONDOWN:
			POINT pt;
			GetCursorPos(&pt);
			HMENU hMenu;
			hMenu = CreatePopupMenu();
			AppendMenuA(hMenu, MF_STRING, IDM_SHOW, "显示");
			AppendMenuA(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuA(hMenu, MF_STRING, IDM_ABOUT, "关于");
			AppendMenuA(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuA(hMenu, MF_STRING, IDM_HIDE, "隐藏");
			AppendMenuA(hMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuA(hMenu, MF_STRING, IDM_EXIT, "退出");
			AppendMenuA(hMenu, MF_SEPARATOR, 0, NULL);
			TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hwnd, NULL);
			break;
		default:
			break;
		}
		return 0;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDM_SHOW:
			ShowWindow(hwnd, SW_SHOWNORMAL);
			break;
		case IDM_HIDE:
			ShowWindow(hwnd, SW_HIDE);
			break;
		case IDM_ABOUT:
			MessageBoxA(hwnd, "逼哥和威哥真爱!", "逼哥爱威哥！", MB_OKCANCEL);
			break;
		case IDM_EXIT:
			SendMessage(hwnd, WM_CLOSE, wParam, lParam);
			break;
		}
		return 0;

	case WM_SETTINGCHANGE:
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		f24Hour = (szBuffer[0] == '1');

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');

		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 276, 72, NULL);
		SetViewportExtEx(hdc, cxClient, cyClient, NULL);

		SetWindowExtEx(hdc, 138, 36, NULL);
		SetViewportExtEx(hdc, cxClient / 2, cyClient / 2, NULL);

		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, hBrushRed);

		DisplayTime(hdc, f24Hour, fSuppress);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		DeleteObject(hBrushRed);
		PostQuitMessage(0);
		Shell_NotifyIcon(NIM_DELETE, &nti);
		return 0;

	default:
		/*
		* 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
		*
		* 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
		* 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
		* 统托盘的图标即可。
		*/
		if (message == WM_TASKBARCREATED)
			SendMessage(hwnd, WM_CREATE, wParam, lParam);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


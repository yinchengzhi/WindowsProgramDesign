
#include"TimePower.h"

double GetDlgItemDouble(HWND hwnd, UINT nID)
{
	char doubleNumber[256];
	GetDlgItemTextA(hwnd, nID, doubleNumber, sizeof(doubleNumber));
	return atof(doubleNumber);
}
//由字符串转换为整数: atoi  字符串转化为浮点数: atof
//由整数转换为字符串: itoa  没有浮点数转化为字符串: (ftoa)
//sprintf函数能将各种类型的数字转化为字符串：格式化函数

void SetDlgItemDouble(HWND hwnd, UINT nID, double number)
{
	char doubleNumber[256];
	sprintf(doubleNumber, "%lf", number);
	SetDlgItemTextA(hwnd, nID, doubleNumber);
}

INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{//消息回调函数
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			nHour = GetDlgItemInt(hwndDlg, IDC_HOUR, NULL, TRUE);
			nMinute = GetDlgItemInt(hwndDlg, IDC_MINUTE, NULL, TRUE);
			nSecond = GetDlgItemInt(hwndDlg, IDC_SECOND, NULL, TRUE);
			if (nHour < 0 || nHour > 23)
			{
				MessageBoxA(hwndDlg, "小时能够为负数或者大于23吗？", "提示", MB_OK);
				//SetDlgItemTextA(hwndDlg, ID_TIME, " 时间不能小于0!");
			}
			else if (nMinute > 60 || nMinute < 0)
			{
				MessageBoxA(hwndDlg, "分钟能够为负数或者大于60吗？", "提示", MB_OK);
			}
			else if (nSecond > 60 || nSecond < 0)
			{
				MessageBoxA(hwndDlg, "秒能够为负数或者大于60吗？", "提示", MB_OK);
			}
			else
			{
				EndDialog(hwndDlg, IDCANCEL);
				return TRUE;
			}
		}


		if (LOWORD(wParam) == IDCANCEL)
		{
			MessageBoxA(hwndDlg, "确定要退出吗？", "请选择", MB_OKCANCEL | MB_ICONWARNING);
			EndDialog(hwndDlg, IDCANCEL);
			exit(1);
		}

		if (LOWORD(wParam) == ID_AUTHOR)
		{
			MessageBoxA(hwndDlg, "作者：断开魂节！断开锁链!", "无聊之作", MB_OKCANCEL);
		}

		if (LOWORD(wParam) == ID_EXIT)
		{
			EndDialog(hwndDlg, IDCANCEL);
			exit(1);
		}
		break;
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	//hInstance: 资源总管理
	if (DialogBox(hInstance, (LPCTSTR)IDC_MAIN_DLG, NULL, DialogProc)) {
		static TCHAR szAppName[] = TEXT("计时器");
		HWND         hwnd;
		MSG          msg;
		WNDCLASS     wndclass;

		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = NULL;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;

		if (!RegisterClass(&wndclass))
		{
			MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
			return 0;
		}

		//创建普通窗口
		hwnd = CreateWindow(szAppName, TEXT("自动关机器"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInstance, NULL);

		//hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,
		//	szAppName, szAppName, WS_POPUP,
		//	CW_USEDEFAULT, CW_USEDEFAULT,
		//	CW_USEDEFAULT, CW_USEDEFAULT,
		//	NULL, NULL, hInstance, NULL);


		ShowWindow(hwnd, iCmdShow);
		UpdateWindow(hwnd);

		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}

	return 0;
}

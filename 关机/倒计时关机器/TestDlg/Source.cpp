
#include"TimePower.h"

double GetDlgItemDouble(HWND hwnd, UINT nID)
{
	char doubleNumber[256];
	GetDlgItemTextA(hwnd, nID, doubleNumber, sizeof(doubleNumber));
	return atof(doubleNumber);
}
//���ַ���ת��Ϊ����: atoi  �ַ���ת��Ϊ������: atof
//������ת��Ϊ�ַ���: itoa  û�и�����ת��Ϊ�ַ���: (ftoa)
//sprintf�����ܽ��������͵�����ת��Ϊ�ַ�������ʽ������

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
{//��Ϣ�ص�����
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
				MessageBoxA(hwndDlg, "Сʱ�ܹ�Ϊ�������ߴ���23��", "��ʾ", MB_OK);
				//SetDlgItemTextA(hwndDlg, ID_TIME, " ʱ�䲻��С��0!");
			}
			else if (nMinute > 60 || nMinute < 0)
			{
				MessageBoxA(hwndDlg, "�����ܹ�Ϊ�������ߴ���60��", "��ʾ", MB_OK);
			}
			else if (nSecond > 60 || nSecond < 0)
			{
				MessageBoxA(hwndDlg, "���ܹ�Ϊ�������ߴ���60��", "��ʾ", MB_OK);
			}
			else
			{
				EndDialog(hwndDlg, IDCANCEL);
				return TRUE;
			}
		}


		if (LOWORD(wParam) == IDCANCEL)
		{
			MessageBoxA(hwndDlg, "ȷ��Ҫ�˳���", "��ѡ��", MB_OKCANCEL | MB_ICONWARNING);
			EndDialog(hwndDlg, IDCANCEL);
			exit(1);
		}

		if (LOWORD(wParam) == ID_AUTHOR)
		{
			MessageBoxA(hwndDlg, "���ߣ��Ͽ���ڣ��Ͽ�����!", "����֮��", MB_OKCANCEL);
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
	//hInstance: ��Դ�ܹ���
	if (DialogBox(hInstance, (LPCTSTR)IDC_MAIN_DLG, NULL, DialogProc)) {
		static TCHAR szAppName[] = TEXT("��ʱ��");
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

		//������ͨ����
		hwnd = CreateWindow(szAppName, TEXT("�Զ��ػ���"),
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

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
double GetDlgItemDouble(HWND hwnd,UINT nID)
{
	char doubleNumber[256];
	GetDlgItemTextA(hwnd, nID, doubleNumber, sizeof(doubleNumber));
	return atof(doubleNumber);
}
//由字符串转换为整数: atoi  字符串转化为浮点数: atof
//由整数转换为字符串: itoa  没有浮点数转化为字符串: (ftoa)
//sprintf函数能将各种类型的数字转化为字符串：格式化函数

void SetDlgItemDouble(HWND hwnd, UINT nID,double number)
{
	char doubleNumber[256];
	sprintf(doubleNumber, "%lf", number);
	SetDlgItemTextA(hwnd, nID,doubleNumber);
}

void OnInitDialog(HWND hwndDlg)
{
	RECT rect;
	GetWindowRect(hwndDlg, &rect);
	int cx = GetSystemMetrics(SM_CXFULLSCREEN);
	int cy = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (cx - rect.right) / 2;
	int y = (cy - rect.bottom) / 2;
	MoveWindow(hwndDlg, x, y, rect.right, rect.bottom, FALSE);

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
	case WM_INITDIALOG:
	{
		OnInitDialog(hwndDlg);
	}
		break;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK1)
		{
			double nLeft = GetDlgItemDouble(hwndDlg, IDC_LEFT1);
			double nRight = GetDlgItemDouble(hwndDlg, IDC_RIGHT1);
			SetDlgItemDouble(hwndDlg, IDC_RESULT1, nLeft + nRight);
		}

		if (LOWORD(wParam) == IDOK2)
		{
			double nLeft = GetDlgItemDouble(hwndDlg, IDC_LEFT2);
			double nRight = GetDlgItemDouble(hwndDlg, IDC_RIGHT2);
			SetDlgItemDouble(hwndDlg, IDC_RESULT2, nLeft - nRight);
		}

		if (LOWORD(wParam) == IDOK3)
		{
			double nLeft = GetDlgItemDouble(hwndDlg, IDC_LEFT3);
			double nRight = GetDlgItemDouble(hwndDlg, IDC_RIGHT3);
			SetDlgItemDouble(hwndDlg, IDC_RESULT3, nLeft * nRight);
		}

		if (LOWORD(wParam) == IDOK4)
		{
			double nLeft = GetDlgItemDouble(hwndDlg, IDC_LEFT4);
			double nRight = GetDlgItemDouble(hwndDlg, IDC_RIGHT4);
			if (nRight != 0)
				SetDlgItemDouble(hwndDlg, IDC_RESULT4, nLeft / nRight);
			else
				SetDlgItemTextA(hwndDlg, IDC_RESULT4, "除数不能为0");
		}

		if (LOWORD(wParam) == IDOK5)
		{
			int nLeft = GetDlgItemInt(hwndDlg, IDC_LEFT5, NULL, TRUE);
			int nRight = GetDlgItemInt(hwndDlg, IDC_RIGHT5, NULL, TRUE);
			if (nRight != 0)
				SetDlgItemInt(hwndDlg, IDC_RESULT5, nLeft % nRight, TRUE);
			else
				SetDlgItemTextA(hwndDlg, IDC_RESULT5, "除数不能为0");
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			MessageBoxA(hwndDlg, "确定要退出吗？", "请选择", MB_OKCANCEL|MB_ICONWARNING);
			EndDialog(hwndDlg, IDCANCEL);
		}

		if (LOWORD(wParam) == ID_AUTHOR)
		{
			MessageBoxA(hwndDlg, "作者：断开魂节！断开锁链!", "无聊之作", MB_OKCANCEL);
		}

		if (LOWORD(wParam) == ID_EXIT) 
		{
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}

		break;
	default:
		break;
	}
	return 0;     
}

void OnLogin(HWND hwndDlg)
{
	char sName[32], sPass[32];
	GetDlgItemTextA(hwndDlg, IDC_USER, sName, sizeof(sName));
	GetDlgItemTextA(hwndDlg, IDC_PASS, sPass, sizeof(sPass));
	if (strcmp(sName, "admin") || strcmp(sPass, "123456")) 
	{
		SetDlgItemTextA(hwndDlg, IDC_STATUS, "用户名或者密码错误，请重新输入");
		//MessageBoxA(hwndDlg, "用户名或者密码错误，请重新输入", "错误：",MB_OK);
		return;
	}
	EndDialog(hwndDlg, IDOK);
}

INT_PTR CALLBACK LoginProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{//消息回调函数
	switch (uMsg)
	{
	case WM_SETCURSOR:
		if ((HWND)wParam == GetDlgItem(hwndDlg, IDC_USER))
			SetDlgItemTextA(hwndDlg, IDC_STATUS, "请在此处输入账号！");
		if ((HWND)wParam == GetDlgItem(hwndDlg, IDC_PASS))
			SetDlgItemTextA(hwndDlg, IDC_STATUS, "请在此处输入密码！");
		return TRUE;

	case WM_INITDIALOG:
		OnInitDialog(hwndDlg);
		SetDlgItemTextA(hwndDlg, IDC_STATUS, "请输入账号密码.");
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}
		if (LOWORD(wParam) == IDOK)
		{
			OnLogin(hwndDlg);
			return TRUE;
		}
		break;

	default:
		break;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrev,LPSTR lpCmd,int nCmdShow)
{
	//hInstance: 资源总管
	//int cx = GetSystemMetrics(SM_CXVSCROLL);

	if (DialogBox(hInstance, (LPCTSTR)IDD_LOGIN_DLG, NULL, LoginProc) == IDCANCEL)
		return 0;
	DialogBox(hInstance, (LPCTSTR)IDC_MAIN_DLG, NULL, DialogProc);
	return 0;
}

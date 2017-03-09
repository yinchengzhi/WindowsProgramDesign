#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<windowsx.h>
#include"resource.h"
#include<stdio.h>

/*
三种位运算
标志位组合：wParam = MKRBUTTON|MK_CONTROL|MK_SHIFT
标志位的判断：if(wParam & MK_LBUTTION)
标志位的分离：wParam &=~MK_RBUTTON
*/

INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:  //还未显示出来之前要做的准备工作
	{
		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		MoveWindow(hwndDlg, 200, 100, rect.right - rect.left, rect.bottom - rect.top, FALSE);
		SetWindowTextA(hwndDlg, "消息测试");
	}
		break;
	case WM_CLOSE:
		MessageBoxA(hwndDlg, "系统关闭", "测试", MB_OK);
		EndDialog(hwndDlg, IDCANCEL);
		break;
	case WM_LBUTTONDOWN:
	{
		short xPos = LOWORD(lParam);
		short yPos = HIWORD(lParam);
		char s[256];
		sprintf(s, "左键点击了:x=%d,y=%d", xPos, yPos);
		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "测试", MB_OK);
	}
		
		break;
	case WM_RBUTTONDOWN:
	{
		short xPos = LOWORD(lParam);
		short yPos = HIWORD(lParam);
		char s[256];
		sprintf(s, "右键点击了:x=%d,y=%d", xPos, yPos);
		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "测试", MB_OK);
	}
		break;

	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		char s[256];
		sprintf(s, "光标移动到:x=%d,y=%d", pt.x,pt.y);
		if (wParam & MK_LBUTTON)
			strcat(s, "按下左键");
		if (wParam & MK_RBUTTON)
			strcat(s, "按下右键");
		if (wParam & MK_CONTROL)
			strcat(s, "按下control键");
		if (wParam & MK_SHIFT)
			strcat(s, "按下shift");


		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "测试", MB_OK);
	}
	break;
	default:
		break;
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrev,LPSTR lpCmd,int nShow)
{
	DialogBox(hInstance,(LPCWSTR) IDD_MSG_DLG, NULL, DialogProc);
	return 0;
}
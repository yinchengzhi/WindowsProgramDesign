#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<windowsx.h>
#include"resource.h"
#include<stdio.h>

/*
����λ����
��־λ��ϣ�wParam = MKRBUTTON|MK_CONTROL|MK_SHIFT
��־λ���жϣ�if(wParam & MK_LBUTTION)
��־λ�ķ��룺wParam &=~MK_RBUTTON
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
	case WM_INITDIALOG:  //��δ��ʾ����֮ǰҪ����׼������
	{
		RECT rect;
		GetWindowRect(hwndDlg, &rect);
		MoveWindow(hwndDlg, 200, 100, rect.right - rect.left, rect.bottom - rect.top, FALSE);
		SetWindowTextA(hwndDlg, "��Ϣ����");
	}
		break;
	case WM_CLOSE:
		MessageBoxA(hwndDlg, "ϵͳ�ر�", "����", MB_OK);
		EndDialog(hwndDlg, IDCANCEL);
		break;
	case WM_LBUTTONDOWN:
	{
		short xPos = LOWORD(lParam);
		short yPos = HIWORD(lParam);
		char s[256];
		sprintf(s, "��������:x=%d,y=%d", xPos, yPos);
		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "����", MB_OK);
	}
		
		break;
	case WM_RBUTTONDOWN:
	{
		short xPos = LOWORD(lParam);
		short yPos = HIWORD(lParam);
		char s[256];
		sprintf(s, "�Ҽ������:x=%d,y=%d", xPos, yPos);
		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "����", MB_OK);
	}
		break;

	case WM_MOUSEMOVE:
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		char s[256];
		sprintf(s, "����ƶ���:x=%d,y=%d", pt.x,pt.y);
		if (wParam & MK_LBUTTON)
			strcat(s, "�������");
		if (wParam & MK_RBUTTON)
			strcat(s, "�����Ҽ�");
		if (wParam & MK_CONTROL)
			strcat(s, "����control��");
		if (wParam & MK_SHIFT)
			strcat(s, "����shift");


		SetWindowTextA(hwndDlg, s);
		//MessageBoxA(hwndDlg, s, "����", MB_OK);
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
#define _CRT_SECURE_NO_WARNINGS
#include"resource.h"
#include <stdio.h>
#include <time.h>
#include <Windows.h>

#define ID_TIMER 1

//用于时间的存储
extern int nHour;
extern int nMinute;
extern int nSecond;

//用于时间的计算
extern int nH;
extern int nM;
extern int nS;

void DisplayDigit(HDC hdc, int iNumber);

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress);

void DisplayColon(HDC hdc);

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int CreateClock();
#include <tchar.h>
#include <Windows.h>
#include <atlstr.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include <iostream>
#include <string>
using namespace std;

DWORD FindProc(LPCSTR lpName)
{
	DWORD aProcId[1024], dwProcCnt, dwModCnt;
	char szPath[MAX_PATH];
	HMODULE hMod;

	//ö�ٳ����н���ID
	if (!EnumProcesses(aProcId, sizeof(aProcId), &dwProcCnt))
	{
		//cout << "EnumProcesses error: " << GetLastError() << endl;
		return 0;
	}

	//�������н���
	for (DWORD i = 0; i < dwProcCnt; ++i)
	{
		//�򿪽��̣����û��Ȩ�޴�������
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcId[i]);
		if (NULL != hProc)
		{
			//�򿪽��̵ĵ�1��Module��������������Ƿ���Ŀ�����
			if (EnumProcessModules(hProc, &hMod, sizeof(hMod), &dwModCnt))
			{
				GetModuleBaseNameA(hProc, hMod, szPath, MAX_PATH);
				if (0 == _stricmp(szPath, lpName))
				{
					CloseHandle(hProc);
					return aProcId[i];
				}
			}
			CloseHandle(hProc);
		}
	}
	return 0;
}

//��һ������Ϊ�������̵�ӳ�����ƣ���������������в鿴
//�ڶ�������Ϊ��Ҫע���DLL�������ļ���
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Invalid parameters!" << endl;
		return -1;
	}
	//����Ŀ����̣����򿪾��
	DWORD dwProcID = FindProc(argv[1]);
	if (dwProcID == 0)
	{
		cout << "Target process not found!" << endl;
		return -1;
	}
	HANDLE hTarget = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
	if (NULL == hTarget)
	{
		cout << "Can't Open target process!" << endl;
		return -1;
	}

	//��ȡLoadLibraryW��FreeLibrary�����������е���ڵ��ַ
	HMODULE hKernel32 = GetModuleHandle(_T("Kernel32"));
	LPTHREAD_START_ROUTINE pLoadLib = (LPTHREAD_START_ROUTINE)
		GetProcAddress(hKernel32, "LoadLibraryW");
	LPTHREAD_START_ROUTINE pFreeLib = (LPTHREAD_START_ROUTINE)
		GetProcAddress(hKernel32, "FreeLibrary");
	if (NULL == pLoadLib || NULL == pFreeLib)
	{
		cout << "Library procedure not found: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}

	WCHAR szPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, argv[2], -1,
		szPath, sizeof(szPath) / sizeof(szPath[0]));

	//������������ΪLoadLibraryW�Ĳ�������ռ䣬��������ֵд��
	LPVOID lpMem = VirtualAllocEx(hTarget, NULL, sizeof(szPath),
		MEM_COMMIT, PAGE_READWRITE);
	if (NULL == lpMem)
	{
		cout << "Can't alloc memory block: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}

	// ������ΪҪע���DLL���ļ�·��
	if (!WriteProcessMemory(hTarget, lpMem, (void*)szPath, sizeof(szPath), NULL))
	{
		cout << "Can't write parameter to memory: " << GetLastError() << endl;
		VirtualFreeEx(hTarget, lpMem, sizeof(szPath), MEM_RELEASE);
		CloseHandle(hTarget);
		return -1;
	}

	//�����ź�����DLL�������ͨ��ReleaseSemaphore��֪ͨ����������
	HANDLE hSema = CreateSemaphore(NULL, 0, 1, _T("Global\\InjHack"));

	//��DLLע����������
	HANDLE hThread = CreateRemoteThread(hTarget, NULL, 0, pLoadLib, lpMem, 0, NULL);

	//�ͷ����������ڵĲ����ڴ�
	VirtualFreeEx(hTarget, lpMem, sizeof(szPath), MEM_RELEASE);

	if (NULL == hThread)
	{
		cout << "Can't create remote thread: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}

	//�ȴ�DLL�ź��������������˳�
	WaitForSingleObject(hThread, INFINITE);
	HANDLE hObj[2] = { hTarget, hSema };
	if (WAIT_OBJECT_0 == WaitForMultipleObjects(2, hObj, FALSE, INFINITE))
	{
		cout << "Target process exit." << endl;
		CloseHandle(hTarget);
		return 0;
	}
	CloseHandle(hSema);

	//�����߳��˳������ȡDLL��Module ID
	DWORD dwLibMod;
	if (!GetExitCodeThread(hThread, &dwLibMod))
	{
		cout << "Can't get return code of LoadLibrary: " << GetLastError() << endl;
		CloseHandle(hThread);
		CloseHandle(hTarget);
		return -1;
	}

	//�ر��߳̾��
	CloseHandle(hThread);

	//�ٴ�ע��FreeLibrary�������ͷ��������̼��ص�ע����DLL
	hThread = CreateRemoteThread(hTarget, NULL, 0, pFreeLib, (void*)dwLibMod, 0, NULL);
	if (NULL == hThread)
	{
		cout << "Can't call FreeLibrary: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	CloseHandle(hTarget);
	return 0;
}
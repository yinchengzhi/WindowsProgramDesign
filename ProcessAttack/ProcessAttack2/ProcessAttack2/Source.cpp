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

	//枚举出所有进程ID
	if (!EnumProcesses(aProcId, sizeof(aProcId), &dwProcCnt))
	{
		//cout << "EnumProcesses error: " << GetLastError() << endl;
		return 0;
	}

	//遍例所有进程
	for (DWORD i = 0; i < dwProcCnt; ++i)
	{
		//打开进程，如果没有权限打开则跳过
		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcId[i]);
		if (NULL != hProc)
		{
			//打开进程的第1个Module，并检查其名称是否与目标相符
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

//第一个参数为宿主进程的映象名称，可以任务管理器中查看
//第二个参数为需要注入的DLL的完整文件名
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Invalid parameters!" << endl;
		return -1;
	}
	//查找目标进程，并打开句柄
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

	//获取LoadLibraryW和FreeLibrary在宿主进程中的入口点地址
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

	//在宿主进程中为LoadLibraryW的参数分配空间，并将参数值写入
	LPVOID lpMem = VirtualAllocEx(hTarget, NULL, sizeof(szPath),
		MEM_COMMIT, PAGE_READWRITE);
	if (NULL == lpMem)
	{
		cout << "Can't alloc memory block: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}

	// 参数即为要注入的DLL的文件路径
	if (!WriteProcessMemory(hTarget, lpMem, (void*)szPath, sizeof(szPath), NULL))
	{
		cout << "Can't write parameter to memory: " << GetLastError() << endl;
		VirtualFreeEx(hTarget, lpMem, sizeof(szPath), MEM_RELEASE);
		CloseHandle(hTarget);
		return -1;
	}

	//创建信号量，DLL代码可以通过ReleaseSemaphore来通知主程序清理
	HANDLE hSema = CreateSemaphore(NULL, 0, 1, _T("Global\\InjHack"));

	//将DLL注入宿主进程
	HANDLE hThread = CreateRemoteThread(hTarget, NULL, 0, pLoadLib, lpMem, 0, NULL);

	//释放宿主进程内的参数内存
	VirtualFreeEx(hTarget, lpMem, sizeof(szPath), MEM_RELEASE);

	if (NULL == hThread)
	{
		cout << "Can't create remote thread: " << GetLastError() << endl;
		CloseHandle(hTarget);
		return -1;
	}

	//等待DLL信号量或宿主进程退出
	WaitForSingleObject(hThread, INFINITE);
	HANDLE hObj[2] = { hTarget, hSema };
	if (WAIT_OBJECT_0 == WaitForMultipleObjects(2, hObj, FALSE, INFINITE))
	{
		cout << "Target process exit." << endl;
		CloseHandle(hTarget);
		return 0;
	}
	CloseHandle(hSema);

	//根据线程退出代码获取DLL的Module ID
	DWORD dwLibMod;
	if (!GetExitCodeThread(hThread, &dwLibMod))
	{
		cout << "Can't get return code of LoadLibrary: " << GetLastError() << endl;
		CloseHandle(hThread);
		CloseHandle(hTarget);
		return -1;
	}

	//关闭线程句柄
	CloseHandle(hThread);

	//再次注入FreeLibrary代码以释放宿主进程加载的注入体DLL
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
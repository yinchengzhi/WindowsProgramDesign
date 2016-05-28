#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
void ErrorHandling(char *message);
int main(int argc, int argv[]) {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	/* inet_addr函数调用示例*/
	{
		char *addr = "127.212.124.78";
		unsigned long conv_addr = inet_addr(addr);
		if (conv_addr == INVALID_SOCKET)
			printf("Error occured!\n");
		else
			printf("Network ordered integer addr: %#lx \n", conv_addr);
	}

	/*inet_ntoa函数调用示例*/

	{
		struct sockaddr_in addr;
		char *strPtr;
		char strArr[20];

		addr.sin_addr.S_un.S_addr = htonl(0x4e7cd47f);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation3 %s \n", strArr);
	}

	WSACleanup();
	system("pause");
	return 0;
}


void ErrorHandling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
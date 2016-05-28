#include<stdio.h>  
int main1(int   argc, char   *argv[], char   *envp[])
{
	int   i;

	printf("You   have   inputed   total   %d   argments\n", argc);
	for (i = 0; i<argc; i++)
	{
		printf("arg%d   :   %s\n", i, argv[i]);
	}

	printf("The   follow   is   envp   :\n");
	for (i = 0; *envp[i] != '\0'; i++)
	{
		printf("%s\n", envp[i]);
	}
	return   0;
}
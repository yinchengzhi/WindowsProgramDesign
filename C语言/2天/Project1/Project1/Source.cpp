#include<stdio.h>

/*
class Cstudent {
	int nTest;
	void Add(int a, int b) {
		return a + b;
	};
};

class CWorks{
	int nTest;
	void Div(int x, int y) {
		return x / y;
	};
};
*/

int add(int a, int b);
int dec(int a, int b);
int div(int a, int b);

int main() {

	int nLeft;
	int nRight;
	int nResult;
	nLeft = 15;
	nLeft = nLeft + 1;
	nLeft = nLeft + 1;
	nRight = 10;

	nResult = add(nLeft, nRight);
	printf("add���н��: %d\n", nResult);
	nResult = dec(nLeft, nRight);
	printf("dec���н��: %d\n", nResult);
	nRight = nRight - 10;
	nResult = div(nLeft, nRight);
	printf("div���н��: %d\n", nResult);

	getchar();
	return 0;
}
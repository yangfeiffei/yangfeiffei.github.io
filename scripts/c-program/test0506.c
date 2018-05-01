#include <stdio.h>
int main(void){
	int a;
	double b;

	a = b = 1.5;
	// 相当于下面两条语句
	// a = 1.5;
	// b = 1.5;
	// a与b之间没有关系

	printf("a = %d.\n", a);
	printf("b = %.1f.\n", b);

}

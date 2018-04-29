#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	printf("x == y : %d\n", x == y);
	printf("x >= y : %d\n", x >= y);
	printf("x != y : %d\n", x != y);
	printf("x < y : %d\n", x < y);
	printf("(double)x : %f\n", (double)x);
	if ((double)x) printf("if后面可以跟double类型.\n");
	return 0;
}

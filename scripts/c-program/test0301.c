#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	if (x % y)
		puts("B不是A的约数");
	else
		puts("B是A的约数");

	return 0;
}

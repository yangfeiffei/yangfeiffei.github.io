#include <stdio.h>

int main(void){
	int x;
	int y;
	int sum;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	if (x > y) sum=x;
	else sum=y;
	printf("较大的数：%d\n",sum);
	return 0;
}

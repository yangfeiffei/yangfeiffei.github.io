#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	if (x > y)
		printf("较大的数是：%d\n",x);
	else
		printf("较大的数是：%d\n",y);
	return 0;
}

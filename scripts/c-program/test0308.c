// 
// 使用if 计算两个整数的差
//
#include <stdio.h>

int main(void){
	int x;
	int y;
	int z;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);
	
	if (x>y) 
		z=x-y;
	else 
		z=y-x;

	printf("它们的差：%d\n",z);
	return 0;
}

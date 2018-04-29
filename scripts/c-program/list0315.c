// 
// 使用条件表达式计算两个整数的差
//
#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	printf("它们的差：%d\n",(x > y)?x-y:y-x);
	return 0;
}

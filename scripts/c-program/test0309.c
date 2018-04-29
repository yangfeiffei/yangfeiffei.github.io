//
// 条件表达式实现判断两个数是否相等
//
#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	( x == y ) ? puts("B和A相等。"):puts("B和A不相等。");

	return 0;
}

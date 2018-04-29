#include <stdio.h>

int main(void){
	int x;
	int y;
	int sum;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	//条件运算符
	//表达式1？表达式2：表达式3
	//1.判断表达式1的结果
	//2.如果为真（不为0），执行表达式2
	//3.否则，执行表达式3
	
	sum = (x > y)? x:y;
	printf("较大的数：%d\n",sum);
	return 0;
}

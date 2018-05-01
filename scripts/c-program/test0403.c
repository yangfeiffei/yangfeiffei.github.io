/*
 * do while 实现两个整数之间的所有整数的和
 * 	[a,b]
 * 	包括小的数字和大的数字。
 */

#include <stdio.h>

int main(void){
	
	int num1,num2;
	int sum = 0;
	int n;
	
	printf("请输入整数1：");scanf("%d",&num1);
	printf("请输入整数2：");scanf("%d",&num2);
	printf("%d,%d两个整数之间的整数和是：", num1, num2);
	if (num1 >= num2){
		n = num1;
		num1 = num2;
		num2 = n;
	}

	do {
		sum = sum + num1;
		num1 = num1 + 1;
	} while (num1 <= num2);

	printf("%d\n", sum);

	return 0 ;
}

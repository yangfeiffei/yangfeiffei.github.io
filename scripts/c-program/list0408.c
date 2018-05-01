/*
 * 从输入的整数开始倒数到0。
 */
#include <stdio.h>
int main(void){
	
	int num;

	printf("请输入一个整数："); scanf("%d", &num);
	
	while (num >= 0) printf("%d ", num--);
	putchar('\n');

	return 0;

}

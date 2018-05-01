/*
 * 从0数到输入的整数。
 */
#include <stdio.h>
int main(void){
	
	int num;
	int n = 0;

	printf("请输入一个整数："); scanf("%d", &num);
	
	while (n <= num ) printf("%d ", n++);
	putchar('\n');

	return 0;

}

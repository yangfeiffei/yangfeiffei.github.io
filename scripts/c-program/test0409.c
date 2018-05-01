/*
 * 从0 到输入整数之间的偶数。
 */
#include <stdio.h>
int main(void){
	
	int num;
	int n = 1 ;

	printf("请输入一个整数："); scanf("%d", &num);
	
	while (n <= num){
		if (n % 2 == 0) printf("%d ", n);
		n++ ;
	}
	putchar('\n');

	return 0;

}

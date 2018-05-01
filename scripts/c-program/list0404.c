/*
 * do while 实现1-100的和
 */

#include <stdio.h>

int main(void){
	
	int num;
	int sum;
	
	num = 1;
	sum = 0;
	
	do {
		sum = sum + num;
		num = num + 1;
	} while (num <= 100);

	printf("1到100的加和是：%d\n", sum);

	return 0 ;
}

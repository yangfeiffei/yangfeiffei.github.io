/*
 * 显示99乘法表
 */

#include <stdio.h>

int main(void){

	int num;
	int i,j;

	for (i = 1; i <= 9; i ++){
		for (j = 1; j <= 9; j ++){
			printf("%2d\t",j*i);
		}
		putchar('\n');
	}


	return 0 ;
}

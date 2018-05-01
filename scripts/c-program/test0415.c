/*
 * 显示99乘法表
 */

#include <stdio.h>

int main(void){

	int i,j;
	
	// print 1st line
	printf("  |");
	for (i = 1; i <= 9; i ++){
		printf("%3d",i);
	}
	putchar('\n');
	
	// print 2nd line
	for (i = 1; i <= 30; i++){
		printf("-");
	}
	putchar('\n');

	// print 9*9 table
	for (i = 1; i <= 9; i ++){
		// print 1st colume
		printf("%2d|",i);

		for (j = 1; j <= 9; j ++){
			printf("%3d",j*i);
		}
		putchar('\n');
	}


	return 0 ;
}

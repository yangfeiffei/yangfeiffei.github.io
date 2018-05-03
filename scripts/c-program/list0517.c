/*
 * 计算1000以内的质数
 */

#include <stdio.h>

int main(void){
	int i , no ;
	unsigned long counter = 0;

	no = 2;
	printf("%d ", no);

	for ( no = 3; no < 1000; no += 2 ) {
		for ( i = 3; i < no; i += 2 ) {
			counter ++ ;
			if ( no % i == 0 ){
				break;
			}
		}

		if ( no == i ){
			printf("%d ", no);
		}
	}
	putchar('\n');

	printf("运行次数：%lu.\n", counter);

	return 0;
}

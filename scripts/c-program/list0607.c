/*
 * 显示出一个直角在左下方的直角三角形
 */

#include <stdio.h>

void put_stars(int n){
	while ( n-- > 0)
		putchar('*');
}


int main(void){
	int i, ln;
	
	printf("三角形有几层：");
	scanf("%d", &ln);
	for (i =1; i<=ln; i++){
		put_stars(i);
		putchar('\n');
	}

	return 0 ;
}

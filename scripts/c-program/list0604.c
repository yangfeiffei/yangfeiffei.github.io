/*
 * 求4个数的最大值
 */

#include <stdio.h>

int max2( int a, int b ){
	return ( a > b ? a : b );
}

int max4( int a, int b, int c, int d){
	return ( max2( max2(a,b),max2(c,d) ) );
}


int main(void){
	int x, y, w, z;

	printf("整数x:");scanf("%d", &x);
	printf("整数y:");scanf("%d", &y);
	printf("整数w:");scanf("%d", &w);
	printf("整数z:");scanf("%d", &z);

	printf("最大的整数是：%d。\n", max4( x, y, w, z));

	return 0 ;
}

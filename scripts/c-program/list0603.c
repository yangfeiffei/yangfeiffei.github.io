/*
 * 计算两个数的平方差
 */

#include <stdio.h>

int sqr( int x){
	return x * x;
}

int diff( int a, int b){
	return ( a > b ? a- b : b -a );
}

// 一个数的立方
int cube( int a){
	return a * a *a ;
}


int main(void){
	
	int x ,y;

	printf("输入整数x："); scanf("%d", &x);
	printf("输入整数y："); scanf("%d", &y);
    
	printf("这两个数的平方差是：%d。\n", diff( sqr(x), sqr(y) ) );
	printf("x的立方是：%d。\n",cube(x) );
	return 0;
}

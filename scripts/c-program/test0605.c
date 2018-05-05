/* 
 * 输入整数n
 * 显示0-n的所有整数的和
 */

#include <stdio.h>

int sumup( int n){
	int sum = 0;
	//while( n-- >= 0 ){
	//	sum += n;
	//}
	int i;
	for (i = 0; i <= n; i++){
		sum += i;
	}
	return sum;
}


int main(void){

	int a;

	printf("输入一个整数：");
	scanf("%d", &a);

	printf("从1到%d的整数和是：%d。\n",a, sumup(a));

	return 0;
}

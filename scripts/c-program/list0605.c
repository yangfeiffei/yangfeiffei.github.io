/*
 * 计算幂
 */

#include <stdio.h>

double power( double x, int n){

	int i;
	double tmp = 1.0;

	for (i = 0; i < n; i++){
		tmp *= x;
	}

	return tmp;
}



int main(void){
	double a;
	int b;

	printf("实数a：");scanf("%lf", &a);
	printf("整数b：");scanf("%d", &b);

	printf("%.2f的%d次幂是：%.2f。\n",a, b, power(a, b));
}

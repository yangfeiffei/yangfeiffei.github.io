//
// 复合语句，或者程序块，计算两个整数中的大数和小数
// 注意：程序块中可以声明，也可以写语句
//      顺序：先声明，再语句
//
#include <stdio.h>

int main(void){
	int x,y;
	int max,min;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	if(x > y){
		max = x;
		min = y;
	} else {
		max = y;
		min = x;
	}

	printf("较大数：%d\n较小数：%d\n", max, min);

	return 0;
}

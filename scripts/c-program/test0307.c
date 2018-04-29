// 计算4个数中最小的数
#include <stdio.h>

int main(void){
	int x;
	int y;
	int w;
	int z;
	int min;

	puts("输入两个整数：");
	printf("请输入一个整数X：");scanf("%d",&x);
	printf("请输入一个整数Y：");scanf("%d",&y);
	printf("请输入一个整数W：");scanf("%d",&w);
	printf("请输入一个整数Z：");scanf("%d",&z);
	
	min = x;
	if (y < min) min=y;
	if (w < min) min=w;
	if (z < min) min=z;

	printf("最小的数：%d\n",min);
	return 0;
}

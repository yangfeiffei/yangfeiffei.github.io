#include <stdio.h>

int main(void){
	int x;
	int y;
	int z;
	int sum;

	puts("输入两个整数：");
	printf("请输入一个整数X：");scanf("%d",&x);
	printf("请输入一个整数Y：");scanf("%d",&y);
	printf("请输入一个整数Z：");scanf("%d",&z);
	
	sum = x;
	if (y > sum) sum=y;
	if (z > sum) sum=z;

	printf("最大的数：%d\n",sum);
	return 0;
}

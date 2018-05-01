#include <stdio.h>
int main(void){
	int num;
	int i = 1;
	int n;
	int sum = 0;

	printf("输入需要计算的整数个数：");scanf("%d",&num);

	while (num-- > 0) {
		printf("整数%d：",i++);scanf("%d",&n);
		sum += n;
	}
	printf("所有整数的和是：%d.\n",sum);
}

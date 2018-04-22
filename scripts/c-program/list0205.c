#include <stdio.h>
int main(void)
{
	int h;
	double w;
	puts("输入你的身高:");
	printf("身高:"); scanf("%d", &h);
	
	w = (h - 100)*0.9;
	printf("你的标准体重是：%5.1f\n", w);
	
	return (0);
}

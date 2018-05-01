#include <stdio.h>
int main(void)
{
	int h1, h2;
	int i;
	double w;
	puts("输入你的身高:");
	printf("开始数值cm："); scanf("%d", &h1);
	printf("结束数值cm："); scanf("%d", &h2);
	for (i = h1 ; i <= h2; i += 5){
		
		w = (i - 100)*0.9;
		printf("%dcm\t%5.2fkg\n", i, w);
	}	
	
	return (0);
}

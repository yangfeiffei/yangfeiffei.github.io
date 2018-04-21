#include <stdio.h>

int main(void)
{
	int x,y;
	int s;	

	puts("请输入两个整数：");
	
	printf("整数x："); scanf("%d",&x);
	printf("整数y："); scanf("%d",&y);
	s = x * y;
	printf("x和y的乘积是%d。\n", s);
	return (0);
}

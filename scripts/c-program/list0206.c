#include <stdio.h>
int main(void)
{
	int x,y;
	double c;
	puts("输入两个整数:");
	printf("A:"); scanf("%d", &x);
	printf("B:"); scanf("%d", &y);
	c = (double)x/y*100;
	printf("A是B的 %5.2f%%\n", c);;
	
	return (0);
}

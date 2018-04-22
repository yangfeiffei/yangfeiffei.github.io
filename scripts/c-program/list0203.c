#include <stdio.h>
int main(void)
{
	double x,y;
	puts("input two ints:");
	printf("x:"); scanf("%1f", &x);
	printf("y:"); scanf("%1f", &y);
	
	printf("x + y = %f\n", x + y);
	printf("x - y = %f\n", x - y);
	printf("x * y = %f\n", x * y);
	printf("x / y = %f\n", x / y);
	
	return (0);
}

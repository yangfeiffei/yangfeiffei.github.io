#include <stdio.h>
int main(void)
{
	int x,y;
	puts("input two ints:");
	printf("x:"); scanf("%d", &x);
	printf("y:"); scanf("%d", &y);
	
	printf("x / y = %d --- %d\n", x / y, x % y);
	
	return (0);
}

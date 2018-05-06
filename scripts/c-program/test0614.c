#include <stdio.h>

double x;

int main(void){ 
	
	static double y;
	
	printf("x = %f\n", x);
	printf("y = %f\n", y);

	return 0;
}

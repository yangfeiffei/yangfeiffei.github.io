#include <stdio.h>
int main(void)
{
	int x1,x2,x3,x4;
	double y1,y2,y3,y4;
	
	x1 = 5		/2	;
	x2 = 5.0	/2	;
	x3 = 5		/2.0	;
	x4 = 5.0	/2.0	;
					
	y1 = 5		/2	;
	y2 = 5.0	/2	;
	y3 = 5		/2.0	;
	y4 = 5.0	/2.0	;

	printf("x1 = %d\n", x1);
	printf("x2 = %d\n", x2);
	printf("x3 = %d\n", x3);
	printf("x4 = %d\n", x4);
	printf("y1 = %f\n", y1);
	printf("y2 = %f\n", y2);
	printf("y3 = %f\n", y3);
	printf("y4 = %f\n", y4);

	return (0);
}

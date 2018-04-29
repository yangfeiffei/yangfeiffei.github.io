/* 
 * 输入三个数，看是否有两个数相同，或者三个数相同
 *
 */
#include <stdio.h>

int main(void){
	int x, y, z;

	printf("输入整数X："); scanf("%d", &x);
	printf("输入整数Y："); scanf("%d", &y);
	printf("输入整数Z："); scanf("%d", &z);
	
	if (x == y && y == z){
		printf("三个数相等。\n");
	} else if ( x == y || y == z || x == z){
		printf("其中有两个数相等。\n");
	} else {
		printf("三个数各不相同。\n");
	}

	return 0;
}

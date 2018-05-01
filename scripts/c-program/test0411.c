#include <stdio.h>
int main(void){
	int num;
	printf("please input a number:");scanf("%d", &num);
	while (num-- > 0)
		puts("*");
}

#include <stdio.h>

int main(void) {
	int num;
	int dig;
	
	do {
		printf("请输入一个非负整数：");
		scanf("%d",&num);
		if (num < 0 )
			printf("输入的是一个负数。\n");
	}while (num < 0);
	
	dig = 0;

	do {
		num = num / 10;
		dig = dig + 1;
	} while ( num > 0);
	
	printf("该整数的位数是：%d。\n", dig);

	return 0 ;
}

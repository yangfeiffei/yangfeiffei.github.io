#include <stdio.h>

int main(void) {
	int num;
	
	do {
		printf("请输入一个非负整数：");
		scanf("%d",&num);
		if (num < 0 )
			printf("输入的是一个负数。\n");
	}while (num < 0);

	printf("该整数的逆向整数是：");
	do {
		printf("%d", num % 10);
		num = num / 10;
	} while ( num > 0);
	
	puts("。");

	return 0 ;
}

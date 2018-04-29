#include <stdio.h>

int main(void){
	int no;
	printf("请输入一个整数：");
	scanf("%d",&no);

	if (no == 0)
		puts("该整数是0");
	else if (no > 0)
		puts("该整数是正数。");
	else
		puts("该整数是负数。");

	return 0;
}

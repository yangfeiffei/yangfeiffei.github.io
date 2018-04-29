#include <stdio.h>

int main(void){
	int no;

	printf("请输入一个整数：");
	scanf("%d",&no);

	if (no % 2)
		puts("它是奇数。");
	else
		puts("它是偶数。");

	return 0;
}

#include <stdio.h>

int main(void){
	int no;

	printf("请输入一个整数：");
	scanf("%d",&no);

	if (no)
		puts("它不是0。");
	else
		puts("它是0。");

	return 0;
}

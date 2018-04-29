#include <stdio.h>

int main(void){
	int no;
	printf("请输入一个整数：");
	scanf("%d",&no);

	if (no % 10 == 5)
	// %的优先级比==大，所以可以不加括号
		puts("个位数是5。");
	else
		puts("个位数不是5。");

	return 0;
}

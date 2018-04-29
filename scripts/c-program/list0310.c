#include <stdio.h>

int main(void){
	int no;
	printf("请输入一个整数：");scanf("%d",&no);

	if (no > 0) 
		if (no % 2 == 0)
			puts("偶数");
		else
			puts("奇数");
	else
		puts("不是正数。\a\n");
		// \a 表示响铃
	return 0;
}

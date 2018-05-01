#include <stdio.h>

int main(void){
	int no;

	printf("请输入一个整数：");
	scanf("%d",&no);

	switch (no % 2) {
		case 0		: puts("偶数。");		break;
		case 1		: puts("奇数。");		break;
		default		: puts("")	;		break;
	}

	return 0;
}

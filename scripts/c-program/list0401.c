#include <stdio.h>

int main(void) {
	int count;

	do {
		int no;
		printf("输入一个整数："); scanf("%d", &no);
		
		if (no % 2)
			puts("奇数。");
		else
			puts("偶数。");

		printf("退出按9，再来一次按0："); scanf("%d", &count);
	} while (count == 0);

	return  0; 
}


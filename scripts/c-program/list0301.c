#include <stdio.h>

int main(void){
	int no;
	printf("input a int:");
	scanf("%d",&no);

	if (no % 5)
		puts("不能被5整除。");

	return 0;
}

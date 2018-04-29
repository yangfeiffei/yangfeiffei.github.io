#include <stdio.h>

int main(void){
	int no;

	printf("input a int:");
	scanf("%d",&no);

	if (no % 2)
		puts("它是奇数。");

	return 0;
}

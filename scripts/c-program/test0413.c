#include <stdio.h>
int main(void){
	int i;
	int sum = 0 ;
	int num;
	printf("输出数字个数：");scanf("%d",&num);
	for (i = 1 ; i <= num ; i++){
		printf("%d", i % 10);
	}
	putchar('\n');
}

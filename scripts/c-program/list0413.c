#include <stdio.h>
int main(void){
	int i,num;
	printf("请输入一个正整数：");scanf("%d",&num);
	for (i = num ;i > 0; i-- ){
		putchar('*');
	}
	putchar('\n');
}

#include <stdio.h>

int main(void){
	int x;
	int y;

	puts("输入两个整数：");
	printf("请输入一个整数A：");scanf("%d",&x);
	printf("请输入一个整数B：");scanf("%d",&y);

	if (x > y)
		puts("A大于B。");
	else if (x < y)
		puts("A小于B。");
	else
		puts("A等于B。");

	return 0;
}

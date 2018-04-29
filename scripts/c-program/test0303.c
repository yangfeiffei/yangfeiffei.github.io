#include <stdio.h>

int main(void){
	int no,no2;
	printf("请输入一个整数：");scanf("%d",&no);

	if (no >= 0) no2=no;
	else no2=-no;
	printf("绝对值是%d。\n", no2);

	return 0;
}

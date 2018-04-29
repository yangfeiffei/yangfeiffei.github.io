/*
 * 输入两个整数，计算他们的差
 * 判断差小于等于10，还是大于等于11
 */

#include <stdio.h>
int main(void){
	int x,y;
	
	printf("输入整数X："); scanf("%d",&x);	
	printf("输入整数Y："); scanf("%d",&y);	

	if ((x >= y && x - y <= 10) || (x < y && y - x <= 10)) {
		printf("他们的差小于等于10.\n");
	} else {
		printf("他们的差大于等于11.\n");	
	}
}

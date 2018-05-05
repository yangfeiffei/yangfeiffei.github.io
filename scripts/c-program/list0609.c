/*
 * 逆向显示输入的非负整数
 */

#include <stdio.h>

int scan_unit(void){
	
	int no;
	do{
		printf("请输入一个非负整数：");
		scanf("%d", &no);
		if(no < 0){
			puts("\a请不要输入一个负数。");
		}
	} while (no < 0);
	
	return no;
}

int rev_int(int num){
	int tmp = 0;
	if(num > 0){
		do {
			tmp = tmp * 10 + num % 10;
			num = num / 10;
		} while (num > 0);
	}

	return tmp;
}


int main(void){
	int nx = scan_unit();
	printf("%d 反转后的整数是：%d\n", nx, rev_int(nx));
	return 0;
}

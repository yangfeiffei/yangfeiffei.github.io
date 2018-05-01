#include <stdio.h>
int main(void){
	int i;
	int a[5] = {21, 23, 3,};
	int b[5];

	// 注意：赋值的时候不能使用数组直接赋值
	// 如 b = a ; 是错误的
	// 需要逐个对数组的元素进行赋值
	for(i = 0; i < 5; i++){
		b[i] = a[i];
	}

	puts("  a   b ");
	puts("--------");
	for(i = 0; i < 5; i++){
		printf("%4d%4d\n", a[i], b[i]);
	}

	return 0;
}

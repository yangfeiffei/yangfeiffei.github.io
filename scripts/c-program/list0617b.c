/*
 * 确认标识符的作用域
 * 当同名变量分别拥有文件作用域和块作用域的时候
 * 只有最内层的块作用域是有效的
 * 由内而外。。
 * /

#include <stdio.h>

// 文件作用域
int x = 75;

void print_x(void){ 
	printf("x = %d\n", x);
}

int main(void){ 
	
	// 块作用域
	int x = 100;
	int i;

	print_x();  // x=75

	for(i = 0; i < 5; i++){ 
		// 块作用域
		int x = i * 100;
		printf("x = %d\n", x); // x=0,100,--400
	}

	printf("x = %d\n", x); // x=75

	return 0;
	
}

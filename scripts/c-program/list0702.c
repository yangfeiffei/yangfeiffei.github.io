/*
 * 判断char型有无符号
 */

#include <stdio.h>
#include <limits.h>

int main(void){ 

	printf("这个编译器的char型是：");

	if (CHAR_MIN){ 
		printf("有符号的。\n");
	} else { 
		printf("无符号的。\n");
	}

	return 0;
}

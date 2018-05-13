/*
 * 显示字符型和整型的长度
 *  char :1
 *  short:2
 *  int:4
 *  long:8
 *
 *  char 一定是1  sizeof定义就是以char的长度作为1
 *
 *  short ≤ int  ≤  long
 */

#include <stdio.h>
// #include <limits.h>

int main(void){ 
	
	printf("size of char = %d\n",	(unsigned)sizeof(char));
	printf("size of short = %d\n",	(unsigned)sizeof(short));
	printf("size of int = %d\n",	(unsigned)sizeof(int));
	printf("size of long = %d\n",	(unsigned)sizeof(long));
	
	return 0;
}


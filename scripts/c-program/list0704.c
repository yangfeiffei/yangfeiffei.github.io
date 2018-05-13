/*
 * 显示数据类型和变量的长度
 */

#include <stdio.h>

int main(void){ 
	
	int		na,nb;
	double	dx,dy;

	printf("size of int = %u\n",		(unsigned)sizeof(int));
	printf("size of double = %u\n",		(unsigned)sizeof(double));

	printf("size of na = %u\n",		(unsigned)sizeof(na));
	printf("size of dx = %u\n",		(unsigned)sizeof(dx));

	printf("size of na+nb = %u\n",		(unsigned)sizeof(na + nb));
	printf("size of dx+dy = %u\n",		(unsigned)sizeof(dx + dy));
	printf("size of na+dx = %u\n",		(unsigned)sizeof(na + dx));
	
	return 0;
}

/*
 * 显示函数被调用的次数
 * 使用静态存储期
 */


#include <stdio.h>

void put_counts(void){ 
	
	static int count;
	printf("put_count:第%d次。\n", count + 1);
	count++;
}

int main(void){ 

	int i ;

	for (i = 0; i < 3; i++){ 
		put_counts();
	}

	return 0;
}

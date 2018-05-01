#include <stdio.h>
int main(void){
	int i;
	int v[5] = {1, 2, 3, 4, 5, };

	// int v[5];
	// v = {1, 2, 3, 4, 5,};
	// 错误，不能这样进行数组的初始化
	
	for(i = 0; i < 5; i++){
		printf("v[%d] = %d.\n", i, v[i]);
	}

	return 0 ;
}

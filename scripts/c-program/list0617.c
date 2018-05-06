/*
 * 计算 2*3 矩阵之和
 */

#include <stdio.h>

void mat_add(const int ma[2][3], const int mb[2][3], int mc[2][3]){
	//
	// 在声明二维数组的时候可以省略第一维的元素个数，如ma[][3]
	// 不可以使用：ma[][]
	// 不可以使用：*ma
	// const表示常量，不能修改它
	//
	int i, j;

	for (i = 0; i < 2; i++){
		for (j = 0; j < 3; j++){
			mc[i][j] = ma[i][j] + mb[i][j];
		}
	}
}

int main(void){

	int i, j;

	int ma[2][3] = {{1,2,3},{4,5,6}};
	int mb[2][3] = {{4,2,3},{1,5,2}};
	int mc[2][3] = {0};
	mat_add(ma, mb, mc);
	
	for (i = 0; i < 2; i++){
		for (j = 0; j < 3; j++){
			printf("%4d",ma[i][j]);
		}
		putchar('\t');

		for (j = 0; j < 3; j++){
			printf("%4d",mb[i][j]);
		}
		putchar('\t');

		for (j = 0; j < 3; j++){
			printf("%4d",mc[i][j]);
		}
		putchar('\n');
	}
	
	return 0;
}

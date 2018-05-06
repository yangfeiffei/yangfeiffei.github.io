/*
 * 计算2*3 和 3*2 的两个矩阵的乘积
 */

#include <stdio.h>

void mat_multi( const int ma[2][3], const int mb[3][2], int mc[2][2]){

	int i, j, k; // i=j=2,k=3
	for( i = 0; i < 2; i++){
		for ( j = 0; j < 2; j++ ){
			mc[i][j] = 0;
			for ( k = 0; k < 3; k++ ){
				mc[i][j] += ma[i][k] * mb[k][j];
			}
		}
	}
}


int main(void){

	int i, j;

	int ma[2][3] = {{1,2,3},{4,5,6}};
	int mb[3][2] = {{4,2},{1,5},{3,2}};
	int mc[2][2] = {0};

	mat_multi(ma, mb, mc);

	for( i = 0; i < 2; i++){
		for ( j = 0; j < 2; j++ ){
			printf("%4d", mc[i][j]);
		}
		putchar('\n');
	}

	return 0 ;

}



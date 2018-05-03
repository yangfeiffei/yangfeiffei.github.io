#include <stdio.h>

int main(void){
	int i,j;
	int a[4][3];
	int b[3][4];
	int m[4][4] = {0};

	puts("请输入一个4*3的矩阵：");
	for(i = 0; i< 4; i++){
		for(j = 0; j< 3; j++){
			printf("a[%d][%d]",i,j);
			scanf("%d",&a[i][j]);
		}
	}

	puts("请输入一个3*4的矩阵：");
	for(i = 0; i< 3; i++){
		for(j = 0; j< 4; j++){
			printf("b[%d][%d]",i,j);
			scanf("%d",&b[i][j]);
		}
	}

	printf("乘积：\n");
	for(i = 0; i< 4; i++){
		for(j = 0; j< 3; j++){
			m[i][i] += a[i][j] * b[j][i];
		}
	}
	for(i = 0; i< 4; i++){
		for(j = 0; j< 4; j++){

			printf("%4d",m[i][j]);
		}
		putchar('\n');
	}

	
	return 0;
}

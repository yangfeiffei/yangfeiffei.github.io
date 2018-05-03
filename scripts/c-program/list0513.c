#include <stdio.h>

int main(void){

	int i,j;
	int tensu1[4][3] = { {98,84,78},
      			     {78,54,65},
			     {92,53,29},
			     {94,53,69},
		 	   };
	int tensu2[4][3] = { {45,23,76},
			     {65,34,87},
			     {77,94,65},
			     {57,93,63},
			   };
	int sum[4][3];
	
	// 计算两次考试分数之和
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			sum[i][j] = tensu1[i][j] + tensu2[i][j];
		}
	}

	// 显示第1次考试分数
	puts("第1次考试的分数：");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			printf("%4d", tensu1[i][j]);
		}
		putchar('\n');
	}

	// 显示第2次考试分数
	puts("第2次考试的分数：");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			printf("%4d", tensu2[i][j]);
		}
		putchar('\n');
	}
	
	// 显示总分数
	puts("总分：");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			printf("%4d", sum[i][j]);
		}
		putchar('\n');
	}
	
	return 0 ;
}

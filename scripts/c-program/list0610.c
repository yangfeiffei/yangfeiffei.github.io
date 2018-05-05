/*
 * 输入n个学生的成绩，输出最高分
 */

#include <stdio.h>

#define NUMBER 5

int tensu[NUMBER];

int top(void){
	int i;
	int max = tensu[0];

	for( i = 0; i < NUMBER; i++){
		if ( tensu[i] > max ) 
			max = tensu[i];
	}
	return max;
}

int main(void){

	int i;
	printf("请输入%d个学生的成绩：\n", NUMBER);
	for(i = 0; i < NUMBER; i++){
		printf("学生# %d：", i+1);
		scanf("%d", &tensu[i]);
	}
	putchar('\n');

	printf("最高分是：%d.\n", top());

	return 0;
}

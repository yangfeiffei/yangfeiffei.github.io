/*
 * 计算英语和数学的最高分
 */

#include <stdio.h>

#define NUMBER 5

int max_of(int v[], int no){
	int i;
	int max = v[0];
	for(i = 0; i < NUMBER; i++){
		if(v[i] > max)
			max = v[i];
	}

	return max;
}


int main(void){

	int i ;
	int a[NUMBER], b[NUMBER];
	printf("请输入%d位学生的成绩：\n", NUMBER);
	
	for (i = 0; i < NUMBER; i++){
		printf("[%d]数学：", i+1);		scanf("%d",&a[i]);
		printf("    英语：");			scanf("%d",&b[i]);
	}

	printf("数学最高分是：%d.\n", max_of(a, NUMBER));
	printf("英语最高分是：%d.\n", max_of(b, NUMBER));

	return 0;
}

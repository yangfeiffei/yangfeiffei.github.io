#include <stdio.h>

#define NUMBER 5    // 学生的人数

int main(void){
	int i;
	int tensu[NUMBER];
	int max, min;
	
	printf("输入%d个学生的成绩。\n", NUMBER);
	for(i = 0;i < NUMBER; i++){
		printf("%d号：", i + 1);
		scanf("%d", &tensu[i]);
	}
	
	max = min = tensu[0];
	for(i = 0;i < NUMBER; i++){
		if(tensu[i] > max ) max = tensu[i];
		if(tensu[i] < min ) min = tensu[i];
	}
	
	printf("最高分：%5d\n", max);
	printf("最低分：%5d\n", min);

	return 0 ;
}

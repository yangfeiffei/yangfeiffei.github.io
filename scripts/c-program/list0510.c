#include <stdio.h>

#define NUMBER 5    // 学生的人数

int main(void){
	int i;
	int tensu[NUMBER];
	int sum = 0;
	
	printf("输入%d个学生的成绩。\n", NUMBER);
	for(i = 0;i < NUMBER; i++){
		printf("%d号：", i + 1);
		scanf("%d", &tensu[i]);
		sum += tensu[i];
	}
	
	printf("总分：%5d\n", sum);
	printf("平均分：%5.1f\n", (double)sum / NUMBER );

	return 0 ;
}

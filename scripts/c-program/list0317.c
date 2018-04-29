// 计算月份所处季节
// 假设：
// 2，3，4 	春天
// 5，6，7 	夏天
// 8，9，10 	秋天
// 11，12，1	冬天
//
#include <stdio.h>

int main(void){
	int month;
	
	printf("输入月份："); scanf("%d", &month);

	if (month >= 2 && month <= 4){
		puts("春天。");
	} else if ( month >= 5 && month <=7 ){
		puts("夏天。");
	} else if ( month >= 8 && month <= 10){
		puts("秋天。");
	} else if ( month == 11 || month == 12 || month == 1){
		puts("冬天。");
	} else {
		puts("请输入正确的月份，1-12。");
	}


}

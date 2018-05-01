#include <stdio.h>
#define NUMBER 20
int main(void){
	int i,j ;
	int s[NUMBER] = {99,23,24,25,12,100,75,86,56,68,79,76,66,66,44,36,74,72, };
	int v[11] = {0,};
	int max_of_v;

	for(i = 0; i < NUMBER; i++){
		v[s[i] / 10 ] += 1;
	}
	
	// 求出分布数组的最大值
	//
	max_of_v = v[0];
	for(i = 0; i < 11; i++){
		if(max_of_v < v[i]){
			max_of_v = v[i];
		}
		printf("%3d ", v[i]);
	}
	putchar('\n');

	// * 号
	for(j = 0; j < max_of_v; j++){
		for(i = 0; i < 11; i++){
			if(v[i] >= (max_of_v - j) ){
				printf("  * ");
			} else 	{
				printf("    ");
			}
		}
		putchar('\n');
	}
	
	// 倒数第二行的----
	for(i = 0; i < 11; i++){
		printf("----");
	}
	putchar('\n');

	// 最后一行
	for(i = 0; i < 11; i++){
		printf("%3d ",i * 10);
	}
	putchar('\n');


}

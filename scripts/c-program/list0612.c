/*
 * 将数组所有元素置0
 */

#include <stdio.h>

void int_set(int *v, int n){
	int i;
	for (i = 0; i < n; i++){
		v[i] = 0;
	}
}

int main(void){
	int i ;
	int ary1[] = {1,2,3,4,5,6,7,};
	int ary2[] = {4,5,6,7,};
	
	int_set(ary1, 7);
	int_set(ary2, 4);
	
	for(i = 0; i < 7; i++){
		printf("ary1[%d]:%d\n", i, ary1[i]);
	}

	for(i = 0; i < 4; i++){
		printf("ary2[%d]:%d\n", i, ary2[i]);
	}

	return 0;
}

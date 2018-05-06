#include <stdio.h>

int min_of(const int vc[], int no){
	int min = vc[0];
	while ( no-- >0 ){
		min =  min < vc[no] ? min:vc[no];
	}
	return min;
}

void rev_intary( int vc[], int no ){
	int i;

	for ( i = 0; i < no; i++ ){
		printf("%d ", vc[no - 1 - i]);
	}
	putchar('\n');
}

// 
// - const 参数，v2 避免被修改掉
// - 数组可以使用v1[]定义，可以使用 *v1
// - 注意数组的下标记是从0开始的
//
void intary_rcpy(int *v1, const int *v2, int no){
	int i;

	for ( i = 0; i < no; i++ ){
		v1[i] = v2[no - 1 - i];
	}
	
}


int main(void){
	int i;
	int v[10] = { 1,2,3,4,5,6,7,8,9,10,};
	
	// printf("min of v[10] = %d.\n", min_of(v, 10));
	// rev_intary(v, 10);
	
	int v1[10];
	intary_rcpy(v1, v, 10);

	printf("v1:");
	for ( i = 0; i < 10; i++ ){
		printf("%d ", v1[i]);
	}
	putchar('\n');

	return 0 ;

}

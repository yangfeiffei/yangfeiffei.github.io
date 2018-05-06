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


int main(void){
	int v[10] = { 1,2,3,4,5,6,7,8,9,10,};
	
	printf("min of v[10] = %d.\n", min_of(v, 10));

	rev_intary(v, 10);
	
	return 0 ;

}

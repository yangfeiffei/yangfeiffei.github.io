#include <stdio.h>

int min_of(const int vc[], int no){
	int min = vc[0];
	while ( no-- >0 ){
		min =  min < vc[no] ? min:vc[no];
	}
	return min;
}

int rev_intary( int vc[], int no ){
	int r_vc[no] ;
	int i;

	for ( i = 0; i < no; i++ ){
		r_vc[i] = vc[ no -i ];
	}
	return r_vc;
}


int main(void){
	int v[10] = { 1,2,3,4,5,6,7,8,9,10,};
	int rv[10];
	int i;
	
	printf("min of v[10] = %d.\n", min_of(v, 10));

	rv = rev_intary(v, 10);
	
	for( i = 0; i < 10; i++){
		
		printf("%4d ", rv[i]);
	}
	putchar('\n');
	
	
	
	return 0 ;

}

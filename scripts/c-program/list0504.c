#include <stdio.h>
int main(void){
	int i ;
	double v[7];

	for(i = 0; i < 7; i++){
		v[i] = i + 1.0;
	}
	
	for(i = 0; i < 7; i++){
		printf("v[%d] = %.1f.\n", i, v[i]);
	}
	return 0 ;
}

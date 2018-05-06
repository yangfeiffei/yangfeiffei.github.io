/*
 * 顺序查找,sequential search
 * 依次轮询所有元素，直到取到正确的值为止
 */

#include <stdio.h>

#define NUMBER	5
#define FAILED	-1

int search(int *vc, int key, int no){
	/* 
	 * 将vc的no号位置直接定义为key值
	 * 这样不论有没有找到key，都能够在no位置找到一个值
	 * 然后查看这个key的位置，
	 * 得出答案即可。
	 */
	int i;
	vc[no] = key;
	
	// for 后面直接跟分号，就直接将for语句结束了
	for( i = 0; vc[i] != key; i++);

	return i < no ? i:FAILED ;

}

int main(void){

	int i , ky, idx;
	int vx[NUMBER];

	for(i = 0; i < NUMBER; i++){
		printf("vx[%d]:", i); scanf("%d", &vx[i]);
	}

	printf("要查找的值是："); scanf("%d", &ky);

	idx = search(vx, ky, NUMBER);

	if (idx == FAILED) 
		puts("\a查找失败。");
	else
		printf("%d是数组的第%d个元素。\n", ky, idx+1);

	return 0;
}

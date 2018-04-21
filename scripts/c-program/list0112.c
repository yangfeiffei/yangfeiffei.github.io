#include <stdio.h>

int main(void)
{
	int x,y,z;
	int s;	

	puts("请输入3个整数：");
	
	printf("整数x："); scanf("%d",&x);
	printf("整数y："); scanf("%d",&y);
	printf("整数z："); scanf("%d",&z);
	s = x + y + z;
	printf("它们的和是%d。\n", s);
	return (0);
}

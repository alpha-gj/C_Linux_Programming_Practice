#include<stdlib.h>
#include<stdio.h>

int main(void)
{
	const int i1 = 10;
	int *p1 = (int *) i1;
	*p1 = 1000;

	printf("i1 is %d\n");
	return 0;
}

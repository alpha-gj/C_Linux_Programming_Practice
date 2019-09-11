#include<stdlib.h>
#include<stdio.h>

int main(void)
{

// Don't do that
#if 0
	const char *c1 = "hello";
	char *p1 = (char *)c1;

    *(p1) = 'x';                  // segmentation fault
    printf("i1 is %s\n", p1);
	return 0;
#endif

// it works, and it can modify value of const int i1.
#if 1
	const int i1 = 10;
	int *p1 = (int *) &i1;
	*p1 = 1000;

	printf("i1 is %d\n", *p1);
	return 0;
#endif

}

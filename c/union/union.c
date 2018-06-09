#include<stdlib.h>
#include<stdio.h>


union Set{
	int int_output;
	double double_output;
};

int main(void)
{
//	union set set_abc.int_a=123.3;

	union Set set = {30};
	printf("The union variable d of Set is %d\n", set.int_output);
	printf("The union variable lf of Set is %lf\n", set.int_output);
	
	set.double_output = 123.123;
	printf("The union variable d of Set is %d\n", set.double_output);
	printf("The union variable lf of Set is %lf\n", set.double_output);



	printf("Hello World!\n");
	return 0;
}

#include<stdlib.h>
#include<stdio.h>

int main(void)
{
	int arr[4] = {5, 2, 3, 4};
	int* arr_ptr[4];
	int i = 0;

	for (i = 0; i < 4; i++)
	{
		arr_ptr[i] = arr + i;
	}

	printf("Address of (arr) array element\n");
	for (i = 0; i < 4; i++)
	{
		printf("Address of %d index = %p\n", i, arr + i);
	}

	printf("Value of (arr_ptr) array element\n");
	for (i = 0; i < 4; i++)
	{
		printf("Value of %d index = %p\n", i, arr_ptr[i]);
	}

	/* change arr_ptr_string pointer */
	char *arr_ptr_string = "Hello";
	printf("%s\n", arr_ptr_string++);
	printf("%s\n", arr_ptr_string);

	/* */
	int *arr_ptr_gj = NULL;
	arr_ptr_gj = arr;
	for (i = 0; i < 10; i++) 
	{
		printf("The value is %d\n", *arr_ptr_gj);
		printf("address value is %p\n", arr_ptr_gj);
		//arr_ptr_gj = arr_ptr_gj + 1;
		/* regard as arr_ptr_gj ++, don't use arr_prt_gj ++; */
		arr_ptr_gj = ++arr_ptr_gj;	
	}


}

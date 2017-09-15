#include<stdlib.h>
#include<stdio.h>

typedef struct MyStruct MyStruct;

struct MyStruct { 
	int ival;
	float fval;
	double dval;
};

int main(void)
{	
	/* objects  */
	int ival_arr[6][8];
	MyStruct  my_struct_obj[2][3];

	/* pointers */
	//MyStruct *my_struct_obj = &my_struct_obj;
	MyStruct *my_struct_ptr = &(my_struct_obj[1][2]);
	//MyStruct **my_struct_ptrptr  = (MyStruct **)&my_struct_obj;
	//MyStruct **my_struct_ptrptr  = &my_struct_ptr;
	//MyStruct **my_struct_ptrptr  = (MyStruct **) &my_struct_ptr;
	MyStruct **my_struct_ptrptr  = &my_struct_ptr;

	/* fprintf */
	fprintf(stderr, "Size of ival_arr is %d\n", sizeof(ival_arr));
	fprintf(stderr, "Size of my_struct_obj is %d\n", sizeof(my_struct_obj));

	fprintf(stderr, "my_struct_ptr = %x\n", my_struct_ptr);
	fprintf(stderr, "my_struct_ptr + 1 = %x\n", my_struct_ptr + 1);
	fprintf(stderr, "my_struct_ptr + 2 = %x\n", my_struct_ptr + 2) ;
	printf("\n");

	fprintf(stderr, "my_struct_ptrptr = %x\n", my_struct_ptrptr);
	fprintf(stderr, "my_struct_ptrptr + 1 = %x\n", my_struct_ptrptr + 1);
	fprintf(stderr, "my_struct_ptrptr + 2 = %x\n", my_struct_ptrptr + 2) ;
	printf("\n");

	fprintf(stderr, "*my_struct_ptrptr = %x\n", *my_struct_ptrptr);
	fprintf(stderr, "*my_struct_ptrptr + 1 = %x\n", *my_struct_ptrptr + 1);
	fprintf(stderr, "*my_struct_ptrptr + 2 = %x\n", *my_struct_ptrptr + 2) ;
	printf("\n");
/*
	fprintf(stderr, "*my_struct_ptrptr = %x\n", *my_struct_ptrptr);
	fprintf(stderr, "*my_struct_ptrptr + 1 = %x\n", *(my_struct_ptrptr + 1));
	fprintf(stderr, "*my_struct_ptrptr + 2 = %x\n", *(my_struct_ptrptr + 2));
*/

	
}

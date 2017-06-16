#include <stdio.h>
#include <string.h>

/*
   It should print "Hello Deamon" that you expect.
   Actually, the answer is Hello World.

   Point: strlen returns ssize_t which is unsinged, so i value will change to big 
   number (from signed to unsigned) when they are doing operation together.
*/

int main(int argc, char *argv[])
{
	int i = -1;

	if ( i > strlen("Demon"))
		printf("Hello World\n");
	else
		printf("Hello Demon\n");

	return 0 ;
}

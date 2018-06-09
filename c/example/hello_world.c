#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define err_exit(MESSAGE) ( \
	perror(MESSAGE),		\
	exit(1)					\
)

int main(void)
{
	printf("Hello World!\n");
	return 0;
}

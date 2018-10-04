#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define err_exit(MESSAGE) ( \
	perror(MESSAGE),		\
	exit(1)					\
)

int main(void)
{
	const char *string_example = "hello";
	printf("Len of "%s" is %d\n", string_example, strlen(string_example));
	return 0;
}

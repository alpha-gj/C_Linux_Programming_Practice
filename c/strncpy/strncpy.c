#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define err_exit(MESSAGE) ( \
	perror(MESSAGE),		\
	exit(1)					\
)

#define STR_BUFFER_SIZE 5

int main(void)
{
	const char *str = "hello world";
	char str_buffer[STR_BUFFER_SIZE] = {};

	/* Just copy 4 chars, and last one char of buffer is '\0' as end of string */
	if (strncpy(str_buffer, str, sizeof(str_buffer) - 1) == NULL) {

		err_exit("strncpy");

	} else {

		str_buffer[sizeof(str_buffer) - 1] = '\0';
		printf("str_buffer is %s\n", str_buffer); 
	}

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define err_exit(MESSAGE) ( \
	perror(MESSAGE),		\
	exit(1)					\
)

#define DEFAULT_NUM 123

int main(void)
{
	char *p_getenv = getenv("NUM");
	int num = 0;

	if (p_getenv != NULL) {

		/* int */
		num = atoi(p_getenv);
		printf("int getenv is %d\n", num);

		/* char* */
		printf("char getenv is %s\n", p_getenv);

	} else {

		printf("getenv is NULL, default getenv is %d\n", DEFAULT_NUM);

	}
}


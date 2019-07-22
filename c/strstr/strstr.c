#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define err_exit(MESSAGE) ( \
	perror(MESSAGE),		\
	exit(1)					\
)

const char *aws_iot_url = "http://arasens_aws_iot_v0.00.12";
const char *search_aws_iot_name = "arasens_aws_iot";

bool is_aws_iot_app(const char *string)
{
	bool ret = false;
	const char *strstr_result = NULL;

	strstr_result = strstr(string, search_aws_iot_name);

	if (strstr_result != NULL) {	/* find it the string */
		ret = true;
	}

	return ret;
}

int main(void)
{
	printf("is it %s there?\n", search_aws_iot_name);

	if (is_aws_iot_app(aws_iot_url)) {

		printf("yes\n");

	} else {

		printf("no\n");

	}

	return 0;
}

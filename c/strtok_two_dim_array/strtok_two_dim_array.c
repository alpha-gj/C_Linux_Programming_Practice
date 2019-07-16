#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define APP_UPDATE_STRING_SIZE 2
#define APP_UPDATE_STRING_LEN  512

void strtok_app_update_message(char app_update_string[APP_UPDATE_STRING_SIZE][APP_UPDATE_STRING_LEN],
								int app_update_string_array_size,
								const char* published_message)
{
	/* STEP1. Check the arg_time_str is vaild or not */
	if (published_message == NULL) {
		printf("published_message is NULL\n");
		return;
	}

	/* STEP2. Copy the string char to buffer */
	char buf[APP_UPDATE_STRING_SIZE * APP_UPDATE_STRING_LEN];
	strncpy(buf, published_message, APP_UPDATE_STRING_LEN);
	buf[sizeof(buf)-1] = '\0';

	/* STEP3. do strtok */
	const char *delim = ",";
	char *pch = NULL;
	int index = 0;

	pch = strtok(buf, delim);
	while (pch != NULL) {
		strncpy(app_update_string[index++], pch, APP_UPDATE_STRING_LEN);
		pch = strtok (NULL, delim);
	}
}

int main()
{

	const char *published_message = "https://mg51-test-link.s3-ap-southeast-1.amazonaws.com/arasens_gps_monitor_v0.00.02,cd88c8bc1c09ede2fb07832d35e563ca";

	char app_update_string[APP_UPDATE_STRING_SIZE][APP_UPDATE_STRING_LEN] = {};

	strtok_app_update_message(app_update_string, sizeof(app_update_string)/sizeof(app_update_string[0]), published_message);

	printf("%s\n", app_update_string[0]);
	printf("%s\n", app_update_string[1]);

	return 0;
}

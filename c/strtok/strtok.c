#include "string.h"
#include "stdlib.h"
#include "stdio.h"
 

void strtok_time_string(int time_array[], int time_array_size, const char* arg_time_str) 
{
	/* STEP1. Check the arg_time_str is vaild or not */
	if (arg_time_str == NULL) {
		printf("arg_time_str is NULL\n");
		return;
	}	
	
	/* STEP2. Copy the string char to buffer */
	char time_str[8];
	strncpy(time_str, arg_time_str, sizeof(time_str));
	time_str[sizeof(time_str)-1] = '\0';

	/* STEP3. do strtok */
	char *delim = ":";
	char *pch = NULL;
	int index = 0;

	pch = strtok(time_str, delim);
	while (pch != NULL && index != time_array_size) {
		time_array[index++] = atoi(pch); // Change from string to int
		pch = strtok (NULL, delim);
	}      
}

int main()
{
#if 0
	char *start_time_hh_mm[2];  //0:hh, 1:mm
	char *end_time_hh_mm[2];	//0:hh, 1:mm
	int index = 0;

	/* split hh & mm to array */
	printf("first\n");
	char start_time_str[] = "17:35";
	char *delim = ":";
	char * pch;
	pch = strtok(start_time_str,delim);
	while (pch != NULL) {
		start_time_hh_mm[index++] = pch;
		pch = strtok (NULL, delim);
	}      
	printf("%s, %s\n", start_time_hh_mm[0], start_time_hh_mm[1]);

	printf("second\n");
	char end_time_str[] = "18:40";
	pch = NULL;
	pch = strtok(end_time_str,delim);
	index = 0;
	while (pch != NULL) {
		end_time_hh_mm[index++] = pch;
		pch = strtok (NULL, delim);
	}      

	printf("%s, %s\n", end_time_hh_mm[0], end_time_hh_mm[1]);
#else
	char start_time[] = "15:40";
	char end_time[] = "16:40";
	int start_time_hh_mm[2];
	int end_time_hh_mm[2];
	strtok_time_string(start_time_hh_mm, sizeof(start_time_hh_mm)/sizeof(int), start_time);
	strtok_time_string(end_time_hh_mm, sizeof(end_time_hh_mm)/sizeof(int), end_time);
	printf("%d, %d\n", start_time_hh_mm[0], start_time_hh_mm[1]);
	printf("%d, %d\n", end_time_hh_mm[0], end_time_hh_mm[1]);
#endif

  return 0;
}

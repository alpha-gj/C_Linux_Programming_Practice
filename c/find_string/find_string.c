#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(void)
{
	char buffer[] = "what123";
	char *pch;

	pch = strstr(buffer, "what");
	if (pch == NULL) {
		printf("no find it\n")	;
	} else {
		printf("you find it\n");
	}
	
	return 0;
}

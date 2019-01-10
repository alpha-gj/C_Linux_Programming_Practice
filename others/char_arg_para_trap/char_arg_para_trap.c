#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>

using namespace std;

char *get_str()
{
	char str[] = {'a', 'b', 'c', '\0'};
//	printf("get str is %s\n", str);
//	char *str = "192.168.0.101";
	return str;
}

int main(int /*argc*/ , char * /*args*/[])
{
	get_str();
	if (get_str() == NULL) {
		printf("NULL?\n");
	} else {
		printf("%s\n", get_str());
	}

	return 0;
}

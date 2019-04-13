#ifndef _GETOPT_
#define _GETOPT_
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>

/* opt */
extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;

/* common */
#define SIZE 1024
const char *program_name = NULL;

/* ACTION FLAG */
typedef struct {
	bool dir_flag;
	bool speed_flag;
} CAR_ACTION_FLAG;


void showUsage();
bool isArgcIsVaild(int argc, int optind);
bool checkCommandOptions(int argc, char *argv[]);

#endif /* _GETOPT_ */

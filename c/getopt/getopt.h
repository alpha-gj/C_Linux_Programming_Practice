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

#endif /* _GETOPT_ */

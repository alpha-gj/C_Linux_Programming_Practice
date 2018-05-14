#include"getopt.h"

void showUsage()
{
	fprintf(stderr, "usage: %s [-t nsecs] [-n] name\n", program_name);
	return;
}

bool isArgcIsVaild(int argc, int optind)
{
	if (optind >= argc && optind > 1) {
		return true;
	} else {
		return false;
	}
}
bool checkCommandOptions(int argc, char *argv[]) 
{
	//TODO make command_options snprintf process
	//const char *command_options = NULL;
	const char *command_options = "nt:";
	char opt = 0;

	/* Parser option & argument are vaild or not */
	while ((opt = getopt(argc, argv, command_options)) != EOF) {
		switch (opt) {
			case 'n':
				printf("optind = %d\n", optind);
				break;
			case 't':
				printf("optind = %d\n", optind);
				printf("optarg is %s\n", optarg);
				break;
			default:
				printf("optopt = %c\n", optopt); //error opt
				opt = EOF;
				return false;
		}
	}

	/* Check numbers of argc is vaild or not */
	if (isArgcIsVaild(argc, optind)) {
		fprintf(stderr, "Expected argument after options\n");
		return true;
	} else {
		fprintf(stderr, "Un-expected argument after options\n");
		return false;
	}

}

int main(int argc, char *argv[])
{
	/* Initialize opt variables */
	optarg = NULL;
	optind = 1;
	optopt = 0;
	opterr = 0; //disable error output
	program_name = argv[0];

	/* Step1. Check Command options are vaild or not */
	if (checkCommandOptions(argc, argv)) {
		/* Do Nothing */
	} else {
		showUsage();
		exit(EXIT_FAILURE);
	}

	/* Step2. Action */
	//TODO

	/* Step3. Result */
	//TODO
	return 0;
}

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
	const char *command_options = ":nt:";
	char opt = 0;

	/* Parser option & argument are vaild or not */
	while ((opt = getopt(argc, argv, command_options)) != EOF) {

		switch (opt) {
			case 'n':
                printf("opt:-%c\n", opt);
				printf("optind = %d for next, argv[%d]:%s\n", optind, optind, argv[optind]);
				break;
			case 't':
                printf("opt:-%c\n", opt);
				printf("optarg:%s\n", optarg);
				printf("optind = %d for next, argv[%d]:%s\n", optind, optind, argv[optind]);
				break;

            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                fprintf(stderr, "Illegal option: -%c\n", isprint(optopt)?optopt:'#');
				return false;
            case ':':
                fprintf(stderr, "Forget add argument for -%c\n", optopt);
				return false;
			default:
				fprintf(stderr, "Unknown error optopt = %c\n", optopt); //error opt
				opt = EOF;
				return false;
		}
	}

	/* Check numbers of argc is vaild or not */
	if (isArgcIsVaild(argc, optind)) {
		fprintf(stderr, "Expected numbers of argument after options\n");
		return true;
	} else {
		fprintf(stderr, "Un-expected numbers of argument after options\n");
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
	}

	printf("=== argv start ===\n");     

	for (int i = 0; i < argc; i++) { 
		fprintf(stderr, "%s ", argv[i]);
	}

	printf("\n=== argv stop ===\n");


    printf("\n-------\n");
    printf("argc:%d, optind:%d\n", argc, optind);

    if (argc > optind) {
        int i = 0;
        for (i = optind; i < argc; i++) {
            fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
        }
    }

    exit(EXIT_FAILURE);

	/* Step2. Action */
	//TODO

	/* Step3. Result */
	//TODO
	return 0;
}

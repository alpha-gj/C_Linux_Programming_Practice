#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUFF_SIZE
#define BUFF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
	bool ret = false;
	int inputFd = -1, outputFd = -1, openFlags = -1;
	mode_t filePerms = 0; 
	size_t numRead = 0;
	size_t numWrite = 0;
	char buf[BUFF_SIZE] = {0};
		
	fprintf(stderr,"%s is starting...\n\n", argv[0]);

	do { 

		/* Check usage */
		if(argc != 3 || strcmp(argv[1], "--help") == 0) {
			fprintf(stderr,"%s [%d] Usage: %s old-file new-file\n",__FUNCTION__,
					__LINE__,  argv[0]);
			exit(EXIT_FAILURE);
		}
		
		/* Open input and output files */
		inputFd = open(argv[1], O_RDONLY);
		if (inputFd == -1 ) {
			fprintf(stderr,"%s [%d] open fail\n", __FUNCTION__, __LINE__);
			break;
		}

		openFlags = O_CREAT | O_WRONLY | O_TRUNC;
		filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
		
		outputFd = open(argv[2], openFlags, filePerms);
		if (outputFd == -1 ) {
			fprintf(stderr,"%s [%d] open fail\n", __FUNCTION__, __LINE__);
			break;
		}

		fprintf(stderr,"%s [%d] filePerms is %o\n",
				__FUNCTION__, __LINE__, filePerms);

		/* Transfer data until we encounter end of input or an error */	
		while ((numRead = read(inputFd, buf, BUFF_SIZE)) > 0) {
			if( (numWrite = write(outputFd, buf, numRead)) != numRead) {
				fprintf(stderr,"%s [%d] couldn't write whole buffer\n",
						__FUNCTION__, __LINE__);
				break;
			}
			fprintf(stderr,"%s [%d] buffer is \n%s\n",
					__FUNCTION__, __LINE__, buf);
			fprintf(stderr,"%s [%d] numRead is %d\n",
					__FUNCTION__, __LINE__, numRead);
			fprintf(stderr,"%s [%d] numWrite is %d\n",
					__FUNCTION__, __LINE__, numWrite);
		}

		printf("After while loop\n");

		fprintf(stderr,"%s [%d] numRead is %d\n",
				__FUNCTION__, __LINE__, numRead);
		fprintf(stderr,"%s [%d] numWrite is %d\n",
				__FUNCTION__, __LINE__, numWrite);

		if (numRead == -1) {
			fprintf(stderr,"%s [%d] read fail\n", __FUNCTION__, __LINE__);
			break;
		}

		ret = true;

	} while(false);

	/* Check fd is close or not */
	if (inputFd != -1) {
			close(inputFd);
			inputFd = -1;
	}
	
	if (outputFd != -1) {
			close(outputFd);
			outputFd = -1;
	}
	
	/* Result */
	fprintf(stderr, "\n\n%s is ending... result is %s!\n",
			argv[0], (ret) ? "OK" : "Fail" );

	exit(EXIT_SUCCESS);
}

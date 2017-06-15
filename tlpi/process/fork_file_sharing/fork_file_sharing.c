#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
//#include <tlpi_hdr.h>



int main(int argc, char *argv[])
{
	int fd, flags;
	char test_template[] = "/tmp/testXXXXXX";
	//char test_template[] = "/home/gj/git/linux_C_practice/tlpi/process/fork_file_sharing";

	setbuf(stdout, NULL);

	fd = mkstemp(test_template); 
	if (fd == -1) {
		perror("mkstemp");
		return -1;
	}
	printf("File offset before fork(); %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

	flags = fcntl(fd, F_GETFL);
	if (flags == -1){
		fprintf(stderr, "fcntl - F_GETFL fail\n");
		return -1;
	}
	printf("O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

	switch(fork()) {
		case -1:
			fprintf(stderr, "fork fail\n");
			return -1;

		case 0:
			if (lseek(fd, 1000, SEEK_SET) == -1) {
				fprintf(stderr, "lssek fail\n");
				return -1;
			}

			flags = fcntl(fd, F_GETFL);
			if (flags == -1) {
				fprintf(stderr, "fcntl - F_GETFL fail\n");
				return -1;
			}

			flags |= O_APPEND;
			if(fcntl(fd, F_SETFL, flags) == -1) {
				fprintf(stderr, "fcntl - F_SETFL fail\n");
				return -1;
			}
			_exit(EXIT_SUCCESS);

		default:
			if (wait(NULL) == -1) {
				fprintf(stderr, "waiting for Child\n");
			}
			printf("Child has exited\n");
				
			printf("File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

			flags = fcntl(fd, F_GETFL);
			if (flags == -1) {
				fprintf(stderr, "fcntl - F_GETFL fail\n");
				return -1;
			}
				fprintf(stderr, "O_APPEND flag in parent is : %s\n", (flags & O_APPEND) ? "on" : "off");
			exit(EXIT_SUCCESS);
	}
}

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <ctype.h>
#include "tlpi_hdr.h"


int main(int argc, char *argv[])
{
	size_t len;
	off_t offset;
	int fd, ap, j;
	char *buf;
	ssize_t numRead, numWritten;
	/* Check usage */
	if(argc < 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr,"%s [%d] Usage: %s 
				file {r<length> | R<length> | w<string> | s<offset>\n",
				__FUNCTION__, __LINE__,  argv[0]);
		exit(EXIT_FAILURE);
	}
	
	fd = open(argv[1], O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
	}

	for (ap = 2, ap < argc; ap++) {
		switch (argv[ap][0]) {
			case 'r': 
			case 'R':
				len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				buf = malloc(len);
				if (buf == NULL) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
				}

				numRead = read (fd, buf, len);
				if (numRead == -1) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
				}

				if (numRead == 0) {
					printf("%s: EOF\n", argv[ap]);
				} else {
					printf("%s:", argv[ap]);
					for (j = 0; j < numRead; j++) {
						if (argv[ap][0] == 'r')
							printf("%c", isprint((unsigned char) buf[j]) ? buf[j] : '?');
						else
							printf("%02x ", (unsigned int) buf [j]);
					}
					printf("\n");
				} 

				free(buf);
				break;
			case 'w':
				numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
				if (numWritten == -1) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
							return -1;
				}
				printf("%s: wrote %ld byte\n", argv[ap],
						(long) numWritten);
				break;

			case 's':
				offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				if (lssek(fd, offset, SEEK_SET) == -1) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
				}
				printf("%s: seek succeeded\n", argv[ap]);
			default:
		}
	}







	printf("Hello World!\n");
	return 0;
	}

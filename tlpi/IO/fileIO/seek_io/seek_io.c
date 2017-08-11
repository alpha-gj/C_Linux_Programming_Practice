#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>
#include "tlpi_hdr.h"
#include "get_num.h"
static void
gnFail(const char *fname, const char *msg, const char *arg, const char *name)
{
    fprintf(stderr, "%s error", fname);
    if (name != NULL)
        fprintf(stderr, " (in %s)", name);
    fprintf(stderr, ": %s\n", msg);
    if (arg != NULL && *arg != '\0')
        fprintf(stderr, "        offending text: %s\n", arg);

    exit(EXIT_FAILURE);
}

static long
getNum(const char *fname, const char *arg, int flags, const char *name)
{
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        gnFail(fname, "null or empty string", arg, name);

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 :
                        (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    res = strtol(arg, &endptr, base);
    if (errno != 0)
        gnFail(fname, "strtol() failed", arg, name);

    if (*endptr != '\0')
        gnFail(fname, "nonnumeric characters", arg, name);

    if ((flags & GN_NONNEG) && res < 0)
        gnFail(fname, "negative value not allowed", arg, name);

    if ((flags & GN_GT_0) && res <= 0)
        gnFail(fname, "value must be > 0", arg, name);

    return res;
}

long
getLong(const char *arg, int flags, const char *name)
{
    return getNum("getLong", arg, flags, name);
}

int
getInt(const char *arg, int flags, const char *name)
{
    long res;

    res = getNum("getInt", arg, flags, name);

    if (res > INT_MAX || res < INT_MIN)
        gnFail("getInt", "integer out of range", arg, name);

    return (int) res;
}


int main(int argc, char *argv[])
{
	size_t len;
	off_t offset;
	int fd, ap, j;
	char *buf;
	ssize_t numRead, numWritten;
	/* Check usage */
	if(argc < 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "%s [%d] Usage: %s file {r<length> | R<length> | w<string> | s<offset> }\n",
				__FUNCTION__, __LINE__,  argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("agrc is %d, agrv[2][0] is %s\n", argc, argv[2]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][0]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][1]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][2]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][3]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][4]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][5]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][6]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][7]);
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][8]);
	
	printf("agrc is %d, agrv[2][0] is %c\n", argc, argv[2][1]);
	
	fd = open(argv[1], O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (fd == -1) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
	} 

	for (ap = 2; ap < argc; ap++) {
		switch (argv[ap][0]) {
			case 'r': 
			case 'R':
				len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
				printf("argv[ap][1] is %c, GN_ANY_BASE is %d\n, argv[ap] is %s\n", argv[ap][1], GN_ANY_BASE, argv[ap]);
				printf("getLong is %d\n", len);
				buf = (char*) malloc(len);
				if (buf == NULL) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
				}

				numRead = read (fd, buf, len);
				if (numRead == -1) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
					return -1;
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
				if (lseek(fd, offset, SEEK_SET) == -1) {
					fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
							__LINE__, strerror(errno));
				}
				printf("%s: seek succeeded\n", argv[ap]);
			default:
				printf("Argument must start with [rRws]: %s\n", argv[ap]);
		}
	}

	if (close(fd) == -1) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__,
				__LINE__, strerror(errno));
	return -1;
	}
	
	return 0;
}

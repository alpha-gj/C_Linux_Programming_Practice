#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <string>

using namespace std;

#define err_exit(MESSAGE) ( \
        perror(MESSAGE),		\
        exit(1)					\
        )

#define POPEN_FREAD_BUFSIZE 4096

string popen_cmd_fread(const char *cmd)
{
    FILE *fp = NULL;
    char buf[POPEN_FREAD_BUFSIZE] = {};
    string result_str;

    do {

        /* STEP1. Check resource validation */
        if (cmd == NULL) {
            printf("cmd is NULL\n");
            break;
        }

        /* STEP2. Execute cmd */
        fp = popen(cmd, "r");
        if (fp == NULL) {
            printf("popen failed\n");
            break;
        }

        /* STEP3. Get result */
		do {
			fread(buf, 1, sizeof(buf), fp);
			if (ferror(fp)) {
				printf("popen ferror!\n");
				break;
			}
		} while (feof(fp) == 0);

        if (buf == NULL) {
            printf("fread buf is NULL\n");
            break;
        }
        buf[sizeof(buf)] = '\0';

        /* STEP4. copy buf to string */
        result_str = buf;

    } while (false);

    /* Free resource */
    if (fp != NULL) {
        if (pclose(fp) == -1) {
            printf("pclose failed\n");
        }
    }

    return result_str;	/* using string copy constructor */
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("指令: %s <linux cmd>\n", argv[0]);
        return 1;
    }

    printf("\n[NOTICE] We use popen() with fread(), and use popen buffer:%d\n", POPEN_FREAD_BUFSIZE);
    printf("you type: %s \n", argv[1]);

    printf("\n====== [Result start] ======\n\n");
    printf("%s", popen_cmd_fread(argv[1]).c_str());
    printf("\n\n====== [Result stop] ======\n");

    return 0;
}

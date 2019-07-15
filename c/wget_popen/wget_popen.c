#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DOWNLOAD_URL "https://mg51-test-link.s3-ap-southeast-1.amazonaws.com/arasens_gps_monitor_v0.00.02"
//TODO It should need to modify
#define DOWNLOAD_DIR_PATH 	"./"
#define ECHO_CHECK			"echo $?"
#define MD5SUM				"cd88c8bc1c09ede2fb07832d35e563ca"


int wget_download_file(
		const char *download_url,
		const char *download_dir_path,
		const char *download_file_name)
{
	int ret = -1;
	char *cmd = NULL;
	FILE *fp = NULL;
	char buf[8] = {};
	char wget_cmd_buf[256] = {};


	/* STEP1. Using resource */
	do {

		if (download_url == NULL ||
			download_dir_path == NULL ||
			download_file_name == NULL) {
			printf("parameter is NULL\n");
			break;
		}

		snprintf(wget_cmd_buf, sizeof(wget_cmd_buf), "wget %s -q -O %s/%s; %s",
				download_url,
				download_dir_path,
				download_file_name,
				ECHO_CHECK);

		if (wget_cmd_buf == NULL) {
			printf("wget_cmd_buf is NULL\n");
			break;
		}

		cmd = strdup(wget_cmd_buf);
		if (cmd == NULL) {
			printf("strdup failed\n");
			break;
		}

		/* Execute cmd */
		fp = popen(cmd, "r");
		if (fp == NULL) {
			printf("popen failed\n");
			break;
		}

		while (fgets(buf, sizeof(buf), fp) != NULL) {
		}
		buf[strcspn(buf, "\n")] = '\0'; // replace "\n" from fgets with "\0"

		if (buf != NULL) {

			if (strncmp(buf, "0", sizeof("0")) == 0) {
				printf("wget download %s: ok\n", download_file_name);
			} else {
				printf("wget download %s: fail\n", download_file_name);
			}

		} else {

			printf("fgets buf is NULL\n");
			break;
		}

		ret = 0;

	} while(false);

	/* STEP2. Free resource */
	if (fp != NULL) {
		if (pclose(fp) == -1) {
			printf("pclose failed\n");
		}
	}

	if (cmd != NULL) {
		free(cmd);
	}

	return ret;
}

bool is_md5sum_of_file_validation(const char *md5sum_value, const char *full_path_file)
{
	bool ret = false;

	char *cmd = NULL;
	FILE *fp = NULL;
	char buf[64] = {};
	char md5_cmd_buf[1024] = {}; //TODO using strdup to prevent that the path is too long to place it


	/* STEP1. Using resource */
	do {

		if (md5sum_value == NULL ||
			full_path_file == NULL) {
			printf("parameter is NULL\n");
			break;
		}

		if (access(full_path_file, F_OK) != 0 ) {
			printf("%s is NOT existed\n", full_path_file);
			break;
		} else {
			printf("%s is existed\n", full_path_file);
		}

		snprintf(md5_cmd_buf, sizeof(md5_cmd_buf), "md5sum '%s' | awk '{print $1}'", full_path_file);
		if (md5_cmd_buf == NULL) {
			printf("md5_cmd_buf is NULL\n");
			break;
		}

		cmd = strdup(md5_cmd_buf);
		if (cmd == NULL) {
			printf("strdup failed\n");
			break;
		}

		/* Execute cmd */
		fp = popen(cmd, "r");
		if (fp == NULL) {
			printf("popen failed\n");
			break;
		}

		while (fgets(buf, sizeof(buf), fp) != NULL) {
		}
		buf[strcspn(buf, "\n")] = '\0'; // replace "\n" from fgets with "\0"

		if (buf != NULL) {

			if (strncmp(buf, md5sum_value, sizeof(buf)) == 0) {
				ret = true;
			}

		} else {

			printf("fgets buf is NULL\n");
			break;
		}


	} while(false);

	/* STEP2. Free resource */
	if (fp != NULL) {
		if (pclose(fp) == -1) {
			printf("pclose failed\n");
		}
	}

	if (cmd != NULL) {
		free(cmd);
	}

	return ret;
}

int main(int argc, char *argv[])
{

	char *download_url = strdup(DOWNLOAD_URL);

	if (download_url == NULL) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	}

	char *filename = basename(download_url);
	if (filename == NULL) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	}

	wget_download_file(DOWNLOAD_URL, ".", filename);


	char full_path_file[256] = {};
	snprintf(full_path_file, sizeof(full_path_file), "%s/%s", ".", filename);

	if (is_md5sum_of_file_validation(MD5SUM, full_path_file)) {

		printf("md5sum validation: ok\n");

	} else {

		printf("md5sum validation: fail\n");
	}

    return 0;
}

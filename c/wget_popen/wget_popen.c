#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <string>

using namespace std;

#define POPEN_BUFSIZE 512

#define DOWNLOAD_URL "https://mg51-test-link.s3-ap-southeast-1.amazonaws.com/arasens_gps_monitor_v0.00.02"
#define DOWNLOAD_DIR_PATH 	"."
#define ECHO_CHECK			"echo $?"
#define FILE_MD5SUM			"cd88c8bc1c09ede2fb07832d35e563ca"

string popen_cmd(const char *cmd)
{
	FILE *fp = NULL;
	char buf[POPEN_BUFSIZE] = {};
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
		while (fgets(buf, sizeof(buf), fp) != NULL) {}
		buf[strcspn(buf, "\n")] = '\0';	// replace '\n' with '\0' because of fgets()
		if (buf == NULL) {
			printf("fgets buf is NULL\n");
			break;
		}

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


string wget_download_file(
		const char *download_url,
		const char *download_dir_path,
		const char *download_file_name)
{
	/* Check validation of parameters */
	string str_result;

	if (download_url == NULL		||
		download_dir_path == NULL	||
		download_file_name == NULL) {
		printf("func parameter is NULL\n");
		return  str_result;
	}

	/* Setup the cmd length for buffer */
	string str_download_url = download_url;
	string str_download_dir_path = download_dir_path;
	string str_download_file_name = download_file_name;

	int wget_cmd_strlen = str_download_url.length() +
						  str_download_dir_path.length() +
						  str_download_file_name.length() +
						  64;	// for wget cmd and option
	char *wget_cmd_buf = NULL;


	/* STEP1. Using resource */
	do {

		/* Create buf for cmd */
		wget_cmd_buf = (char *) malloc(wget_cmd_strlen * sizeof(char));
		if (wget_cmd_buf == NULL) {
			printf("malloc failed\n");
			break;
		}

		/* -q: operate quietly, -O: location where the file is save */
		snprintf(wget_cmd_buf, wget_cmd_strlen, "wget %s -q -O %s/%s; %s",
				download_url,
				download_dir_path,
				download_file_name,
				ECHO_CHECK);

		/* Get the popen result */
		str_result = popen_cmd(wget_cmd_buf);

	} while(false);

	/* STEP2. Free resource */
	if (wget_cmd_buf != NULL) {
		free(wget_cmd_buf);
	}

	return str_result;
}

bool is_md5sum_of_file_validation(const char *md5sum_value, const char *full_path_file)
{
	/* Check validation of parameters */
	bool ret = false;
	string str_result;

	if (md5sum_value == NULL ||
		full_path_file == NULL) {
		printf("parameter is NULL\n");
		return ret;
	}

	if (access(full_path_file, F_OK) != 0) {
		printf("%s is NOT existed\n", full_path_file);
		return ret;
	}

	/* Setup the cmd length for buffer */
	string str_md5sum_value = md5sum_value;
	string str_full_path_file = full_path_file;

	int md5sum_cmd_strlen = str_full_path_file.length() +
							64;	// for md5sum cmd and option
	char *md5sum_cmd_buf = NULL;


	/* STEP1. Using resource */
	do {

		/* Create buf for cmd */
		md5sum_cmd_buf = (char *) malloc(md5sum_cmd_strlen * sizeof(char));
		if (md5sum_cmd_buf == NULL) {
			printf("malloc failed\n");
			break;
		}
		snprintf(md5sum_cmd_buf, md5sum_cmd_strlen, "md5sum '%s' | awk '{print $1}'", full_path_file);

		/* Get the popen result */
		str_result = popen_cmd(md5sum_cmd_buf);

		/* Compare the md5sum is ok? */
		if (strncmp(md5sum_value, str_result.c_str(), str_md5sum_value.length()) == 0)
			ret = true;

	} while(false);

	/* STEP2. Free resource */
	if (md5sum_cmd_buf != NULL) {
		free(md5sum_cmd_buf);
	}
	printf("md5sum from parameter:\t%s\n", md5sum_value);
	printf("md5sum of file:\t\t%s\n", str_result.c_str());

	return ret;
}

int main(int argc, char *argv[])
{
	/* For wget download file */
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
	if (download_url != NULL) {
		free(download_url);
	}

	/* check wget_download_file validation */
	if (strncmp(wget_download_file(DOWNLOAD_URL, DOWNLOAD_DIR_PATH, filename).c_str(), "0", strlen("0")) == 0) {

		printf("wget_download_file: ok\n");

	} else {

		printf("wget_download_file: fail\n");
		exit(0);
	}


	/* For file validation by md5sum */
	char full_path_file[256] = {};
	snprintf(full_path_file, sizeof(full_path_file), "%s/%s", DOWNLOAD_DIR_PATH, filename);

	/* check md5sum validation */
	if (is_md5sum_of_file_validation(FILE_MD5SUM, full_path_file)) {

		printf("md5sum validation: ok\n");

	} else {

		printf("md5sum validation: fail\n");
		exit(0);
	}

    return 0;
}

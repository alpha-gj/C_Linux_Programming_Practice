/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <libgen.h>

using namespace std;

#define SPECIFIC_FILE_PATH  "/var/run/watchdog"
#define EVENT_BUF_LEN       32

int main( )
{

//STEP 1. Create a objective dir and filename

	/* 
	   The POSIX version of dirname and basename may modify the content of the argument. Hence, we need to strdup the local_file.
	   */
	char* ts1 = strdup(SPECIFIC_FILE_PATH);
	char* ts2 = strdup(SPECIFIC_FILE_PATH);

	if (ts1 == NULL || ts2 == NULL) {

		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	}

	char* dir = dirname(ts1);
	char* filename = basename(ts2);

	printf("%s\n", dir);
	printf("%s\n", filename);

	/* Make sure the dir is existed or not */
	if (access(dir, 0) < 0) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	} else {
		printf("%s is exist!\n", dir);    
	}

//STEP 2. inotify it
	int length = -1;
	int fd = -1;
	int wd = -1;
	char buffer[EVENT_BUF_LEN];

	/* Creating the INOTIFY instance*/
	fd = inotify_init();
	if ( fd < 0 ) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	}

	/* Adding the "dir" directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
	wd = inotify_add_watch(fd, dir, IN_ALL_EVENTS);
	if (wd < 0) {
		fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
		exit(0);
	}

	/* Read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 

	bool find_it = false;

	do {

		printf("read...\n");
		length = read(fd, buffer, EVENT_BUF_LEN); 

		/* Checking for error*/
		if (length < 0 ) {
			fprintf(stderr,"%s [%d] %s\n", __FUNCTION__, __LINE__, strerror(errno));
			exit(0);
		}  

		printf("%s", buffer);
		struct inotify_event *event = ( struct inotify_event * ) &buffer;

		if (event->mask & IN_DELETE && string(event->name).compare(filename) == 0) {
			printf( "New filename %s delete.\n", event->name);
			find_it = true;
		}
		if (event->mask & IN_CREATE && string(event->name).compare(filename) == 0) {
			printf( "New filename %s create.\n", event->name);
			find_it = true;
		}

		printf("event->mask: %d\n", event->mask);
		printf("event->name: %s\n", event->name);

	} while (!find_it);

	/* Removing the “dir” directory from the watch list.*/
	printf("inotify_rm_watch\n");
	inotify_rm_watch(fd,wd);

	/* Closing the INOTIFY instance*/
	if (fd > 0)
		close(fd);

	if (ts1 != NULL)
		free(ts1);

	if (ts2 != NULL)
		free(ts2);
}

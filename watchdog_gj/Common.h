#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include "Variable.h"
#include "todo.h"
#include "MulCast/MulCast.h"

enum LIGHT_MODE {
	UNDER_DAY_MODE,
	UNDER_NIGHT_MODE,
	UNDER_ALWAYS_DAY_MODE,
	UNDER_ALWAYS_NIGHT_MODE,
};

enum IR_CONTORLLER {
	IR_AUTO_BY_LIGHT,
	IR_MANUAL,
	IR_OFF_ALWAYS_DAY,
	IR_ON_ALWAYS_NIGHT,
};

int start_thread(void *callback, pthread_t & pid, void *pass);
int start_thread_join(void *callback, pthread_t & pid, void *pass);

bool get_quit();
void set_quit(bool b);
bool get_reload();
void set_reload(bool b);

void send_ipccmd(const char *path, int ipc);

struct timeval get_uptime();
long diff_curr_uptime_count(timeval t);

#endif
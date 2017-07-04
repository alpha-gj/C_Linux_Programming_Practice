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
#include "i2c_drv.h"
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

struct audio_det_struct {
	pthread_t pid;
	//mic
	char mic_enable;
	int volume;
	// alert
	char alert_enable;
	unsigned int threshold;
	// reserve
	char suspend_flag;
	bool need_exit;
};

int start_thread(void *callback, pthread_t & pid, void *pass);
int start_thread_join(void *callback, pthread_t & pid, void *pass);
int does_eth_plug();

void nfc_handler(int signum);
int get_nfc_count();
void set_nfc_count(int n);

int open_i2c_device();
int close_i2c_device(int fd);

bool get_quit();
void set_quit(bool b);
bool get_reload();
void set_reload(bool b);

LIGHT_MODE get_light_mode();
void set_light_mode(LIGHT_MODE m);

void send_ipccmd(const char *path, int ipc);

int get_volume();
void set_volume(int v);

void *run_calling_out_thread(void *args);

struct timeval get_uptime();
long diff_curr_uptime_count(timeval t);

void SwitchAttenuation(bool on);
void *run_alert_det_thread(void *args);
void setSpeakerMute(bool mute);
void setMicMut(bool mute);
#endif

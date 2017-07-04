#include "Common.h"
#include "command.h"
#include "ipc/ipc_cmd.h"
#include "TwSpiLib.h"
#include "mxuvc/mxuvc.h"

int nfc_count = 0;
bool is_quit = false;
bool is_reload = false;
LIGHT_MODE light_mode = UNDER_DAY_MODE;
int volume = 0;

int start_thread(void *callback, pthread_t & pid, void *pass)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&attr, 1048576);
	pthread_create(&pid, &attr, (void *(*)(void *))callback, pass);
	pthread_attr_destroy(&attr);
	return 0;
}

int start_thread_join(void *callback, pthread_t & pid, void *pass)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_attr_setstacksize(&attr, 1048576);
	pthread_create(&pid, &attr, (void *(*)(void *))callback, pass);
	pthread_attr_destroy(&attr);
	return 0;
}

int does_eth_plug()
{
	struct sockaddr_nl src_addr, dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	int sock_fd;
	struct msghdr msg;

	sock_fd = socket(PF_NETLINK, SOCK_RAW,NETLINK_USERSOCK);
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();  /* self pid */
	src_addr.nl_groups = 0;  /* not in mcast groups */
	bind(sock_fd, (struct sockaddr*)&src_addr,sizeof(src_addr));
	memset(&msg,0,sizeof(msg));
	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;   /* For Linux Kernel */
	dest_addr.nl_groups = 0; /* unicast */

	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(int)));
	/* Fill the netlink message header */
	nlh->nlmsg_len = NLMSG_SPACE(sizeof(int));
	nlh->nlmsg_pid = getpid();  /* self pid */
	nlh->nlmsg_flags = 0;
	/* Fill in the netlink message payload */
	*(int *)NLMSG_DATA(nlh) = 0;

	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	sendmsg(sock_fd, &msg, 0);

	//printf("Waiting for message from kernel\n");

	/* Read message from kernel */
	recvmsg(sock_fd, &msg, 0);

	//printf("Received message payload: %d\n",*(int *)NLMSG_DATA((msg.msg_iov)->iov_base));
	close(sock_fd);
	return *(int *)NLMSG_DATA((msg.msg_iov)->iov_base);
}

void nfc_handler(int /*sigNum */ )
{
	++nfc_count;
}

int get_nfc_count()
{
	return nfc_count;
}

void set_nfc_count(int n)
{
	nfc_count = n;
}

int open_i2c_device()
{
	char device[16] = {0};
	snprintf(device, sizeof(device), "/dev/%s", I2C_DEV_NAME);
	return open(device, O_WRONLY);
}

int close_i2c_device(int fd)
{
	if (fd > 0) {
		close(fd);
	}
	return 0;
}

bool get_quit()
{
	return is_quit;
}

void set_quit(bool b)
{
	is_quit = b;
}

bool get_reload()
{
	return is_reload;
}

void set_reload(bool b)
{
	is_reload = b;
}

LIGHT_MODE get_light_mode()
{
	return light_mode;
}

void set_light_mode(LIGHT_MODE m)
{
	light_mode = m;
}

void send_ipccmd(const char *path, int ipc)
{
	IpcClient client;
	IpcResponse res;
	IpcCommand cmd = {ipc, 0, 0};
	client.send_cmd(path, &cmd, sizeof(cmd), &res, sizeof(res));
}

int get_volume()
{
	return volume;
}

void set_volume(int v)
{
	volume = v;
}

void *run_calling_out_thread(void *args)
{
	bool *needStopCallingOut = (bool *)args;
	int callingOutSinkInputIndex = 0;
	char buf[512] = {0};
	char cmd[128] = {0};
	SwitchAttenuation(false);
	while(!*needStopCallingOut) {
		snprintf(cmd, sizeof(cmd), PLAY_CALL_OUT_TONE " %d", 
				get_volume() * 65536 / 100);
		FILE *f = popen(cmd, "r");
		if (!f) {
			ERROR("try to play call out tone failed.");
			break;
		}
		fread(buf, sizeof(buf), 1, f);
		sscanf(buf, "Playing on sink input #%d", 
				&callingOutSinkInputIndex);
		//fprintf(stderr, "the sink index is %d\n", callingOutSinkInputIndex);
		pclose(f);
		for (int i = 0; i < 100; ++i) {
			usleep(61170);
			if (*needStopCallingOut) {
				break;
			}
		}
	}
	if (callingOutSinkInputIndex != 0) {
		snprintf(cmd, sizeof(cmd), "pacmd kill-sink-input %d", callingOutSinkInputIndex);
		//fprintf(stderr, "%s\n", cmd);
		system(cmd);
	} else {
		ERROR("cannot find the sink-input index to stop");
	}
	SwitchAttenuation(true);
	pthread_exit(NULL);
}

struct timeval get_uptime()
{
	struct timespec t = {0, 0};
	struct timeval ret = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &t);
	ret.tv_sec = t.tv_sec;
	ret.tv_usec = t.tv_nsec / 1000;
	return ret;
}

long diff_curr_uptime_count(struct timeval t)
{
	struct timeval current = get_uptime();
	struct timeval res = {0, 0};
	timersub(&current, &t, &res);
	return ((long long)res.tv_sec * 1000000 + res.tv_usec) * TIME_BASE / 1000000;
}

void SwitchAttenuation(bool on)
{
	TwSpiLib t;
	do {
		if (t.init(0x300, 14, on)) {
			fprintf(stderr, "tw spi lib init fail\n");
			break;
		}
		t.doIt();
		t.release();
	} while(false);
}

void* run_alert_det_thread(void *args)
{
	struct audio_det_struct *det = (struct audio_det_struct *)args;
	struct timeval tv;
	unsigned int db = 0;
	int ret = 0;
	MultiSender sender;
	sender.Init(DEFAULT_CHANNEL);
	MultiCastPackage packet;
	memset((unsigned char *)&packet, 0x00, sizeof(MultiCastPackage));
	while(!det->need_exit){
		ret = mxuvc_get_current_audio_intensity(&db);
		//fprintf(stderr, "ret %d, db %d\n", ret, db);
		if (!ret &&
				!det->suspend_flag) {
			gettimeofday(&tv, NULL);
			packet.iSec = tv.tv_sec;
			packet.iUsec = tv.tv_usec;
			packet.iRegion = db;
			packet.iEventMask = SOUND_LEVEL_EVENT;
			packet.iOnOff = false;
			sender.SendPackage(&packet);
			if ((db >= det->threshold) &&
					det->alert_enable &&
					det->mic_enable) {
				packet.iEventMask = AUDIO_DETECTION_EVENT;
				packet.iOnOff = true;
				sender.SendPackage(&packet);
			}
		}
		sleep(1);
	}
	sender.Release();
	pthread_exit(EXIT_SUCCESS);
}

void setSpeakerMute(bool mute)
{
	if (mute) {
		system("/usr/bin/setupAudio -om 1");
	} else {
		system("/usr/bin/setupAudio -om 0");
	}
}

void setMicMut(bool mute)
{
	if (mute) {
		system("/usr/bin/setupAudio -im 1");
	} else {
		system("/usr/bin/setupAudio -im 0");
	}
}

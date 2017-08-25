#include "Common.h"
#include "command.h"
#include "ipc/ipc_cmd.h"

bool is_quit = false;
bool is_reload = false;

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

void send_ipccmd(const char *path, int ipc)
{
	IpcClient client;
	IpcResponse res;
	IpcCommand cmd = {ipc, 0, 0};
	client.send_cmd(path, &cmd, sizeof(cmd), &res, sizeof(res));
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


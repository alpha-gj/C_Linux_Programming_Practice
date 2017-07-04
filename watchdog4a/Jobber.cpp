#include "Jobber.h"
#include "PowerUpHandler.h"
Jobber *job = NULL;

Jobber::Jobber()
{
}

Jobber::~Jobber()
{
}

int Jobber::init()
{
	int ret = -1;
	do {
		holder = StatesHolder::CreateStatesHolder();
		if (!holder) {
			ERROR("can not alloc holder @ %d", __LINE__);
			break;
		}
		ret = 0;
	} while(false);
	return ret;
}

int Jobber::run()
{
	while(!get_quit() && !get_reload()) {
		IPCHandler *handler = CreateHandlerByStates();
		if (handler) {
			if (!handler->init()) {
				handler->run_parsing_command();
			}
			handler->release();
			delete handler;
		} else {
			INFO("BUG@%d", __LINE__);
			sleep(1);
		}
	}
	return 0;
}

int Jobber::release()
{
	if (holder) {
		StatesHolder::releaseStatesHolder();
	}
	return 0;
}

IPCHandler *Jobber::CreateHandlerByStates()
{
	switch (holder->GetMainStates()) {
		case POWERUP:
			INFO("enter POWERUP");
			return new PowerUpHandler();
			break;
		default:
			INFO("BUG@%d", __LINE__);
			return new PowerUpHandler();
			break;
	}
	return NULL;
}
/****************************************************
 *
 *
 *		main function
 *
 *
 */

void requestExitHandler(int sigNum)
{
	set_quit(true);
	INFO("catch signal[%d], set quit %d", sigNum, get_quit());
}

void requestReloadHandler(int sigNum)
{
	set_reload(true);
	INFO("catch signal[%d], set reload %d", sigNum, get_reload());
}

bool initSignal()
{
	// init signal
	sighandler_t sigResult;
	sigResult = signal(SIGINT, requestExitHandler);
	if (sigResult == SIG_ERR) {
		return false;
	}
	sigResult = signal(SIGTERM, requestExitHandler);
	if (sigResult == SIG_ERR) {
		return false;
	}
	sigResult = signal(SIGHUP, requestExitHandler);
	if (sigResult == SIG_ERR) {
		return false;
	}
	sigResult = signal(SIGPIPE, SIG_IGN);
	if (sigResult == SIG_ERR) {
		return false;
	}
	sigResult = signal(SIGUSR1, requestReloadHandler);
	if (sigResult == SIG_ERR) {
		return false;
	}
	sigResult = signal(SIGUSR2, SIG_IGN);
	if (sigResult == SIG_ERR) {
		return false;
	}
	return true;
}

bool create_pidfile(const char *path)
{
	char line[40];
	flock lock;
	int pid_fd =
		open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	bool ret = false;
	if (pid_fd < 0)
		perror("open");
	else {
		lock.l_type = F_WRLCK;
		lock.l_start = 0;
		lock.l_whence = SEEK_SET;
		lock.l_len = 0;
		if (fcntl(pid_fd, F_SETLK, &lock) < 0) {
			if (errno == EACCES || errno == EAGAIN)
				ERROR("Program already exists.");
			else
				ERROR("Unable to lock %s", path);
		} else {
			snprintf(line, sizeof(line), "%ld", (long)getpid());
			ftruncate(pid_fd, 0);
			write(pid_fd, line, strlen(line));
			ret = true;
		}
	}
	return ret;
}

int main(int /*argc*/, const char *argv[])
{
	if (!initSignal()) {
		ERROR("init signal handler failed.");
		return EXIT_FAILURE;
	}
	/*
	   if (!create_pidfile(JOBBER_PID_FILE_PATH)) {
	   ERROR("create_pidfile failed");
	   return EXIT_FAILURE;
	   }
	   */
	openlog("JOB4", LOG_PID | LOG_NDELAY, 0);
	do {
		job = new Jobber();
		if (job) {
			if (!job->init()) {
				job->run();
			} else {
				sleep(1);
			}
			job->release();
			delete job;
		} else {
			ERROR("can not alloc memory for job");
			sleep(1);
		}
	} while (!get_quit());
	closelog();
	//unlink(JOBBER_PID_FILE_PATH);
	return EXIT_SUCCESS;
}


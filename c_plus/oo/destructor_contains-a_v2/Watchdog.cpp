#include "Watchdog.h"
#include "PowerUpHandler.h"
#include "watcher/watcher.h"
Watchdog *dog = NULL;

Watchdog::Watchdog()
{
}

Watchdog::~Watchdog()
{
}

int Watchdog::init()
{
	int ret = -1;
	do {
		//TODO, maybe gpio init here
		holder = StatesHolder::CreateStatesHolder();
		if (!holder) {
			ERROR("can not alloc holder @ %d", __LINE__);
			break;
		}
		ret = 0;
	} while(false);
	return ret;
}

int Watchdog::run()
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

int Watchdog::release()
{
	if (holder) {
		StatesHolder::releaseStatesHolder();
	}
	return 0;
}

IPCHandler *Watchdog::CreateHandlerByStates()
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

int main(int /*argc*/, const char *argv[])
{
	int process_stat = watcher(argv[0], "/etc/rc.d/init.d/watchdog.sh");
	if (process_stat == PARENT_PROCESS) {
		return 0;
	} else if (process_stat == CHILD_PROCESS) {
		if (!initSignal()) {
			ERROR("init signal handler failed.");
			return EXIT_FAILURE;
		}
		openlog("JOB4", LOG_PID | LOG_NDELAY, 0);
		do {
			dog = new Watchdog();
			if (dog) {
				if (!dog->init()) {
					dog->run();
				} else {
					sleep(1);
				}
				dog->release();
				delete dog;
			} else {
				ERROR("can not alloc memory for dog");
				sleep(1);
			}
		} while (!get_quit());
		closelog();
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "fork failed\n");
		return -1;
	}
}


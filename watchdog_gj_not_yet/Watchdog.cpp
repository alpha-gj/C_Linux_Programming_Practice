#include "Watchdog.h"
using namespace std;
Watchdog *dog = NULL;

Watchdog::Watchdog():hw_manager(NULL),holder(NULL)
{
	cout << "*** Jobber constructer without pra is running ... ***" << endl;
		hw_manager = HwManager::CreateHwManager();
		if (!hw_manager) {
			fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
		}

		holder = StatesHolder::CreateStatesHolder();
		if (!holder) {
			fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
		}

}

Watchdog::~Watchdog()
{
	//make sure source is free or not?
	// check holder or handler
	cout << "*** Jobber destructer is running ... ***" << endl;
	if (holder) {
		StatesHolder::ReleaseStatesHolder();
		holder = NULL;
	}

	if (hw_manager) {
		HwManager::ReleaseHwManager();
		hw_manager = NULL;
	}
}

int Watchdog::init()
{
	return -1;
}

int Watchdog::deinit()
{
	return -1;
}

int Watchdog::run()
{
#if 0
	while(!get_quit() && !get_reload()) {
		IPCHandler *handler = CreateHandlerByStates();
		if (handler) {
			handler->run_parsing_command();
			delete handler;
			handler = NULL;
		} else {
			INFO("BUG@%d", __LINE__);
			sleep(1);
		}
	}
#else

	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};
	hw_manager->get_hw_info_by_type("BUTTON", &button_setting);
	
	if(button_setting.status == AHAL_BTN_STATUS_PRESSED)
		printf("RESET PRESSED!!!!!!!!!!!\n");
	else if (button_setting.status == AHAL_BTN_STATUS_RELEASE)
		printf("RESET RELEASE!!!!!!!!!!!\n");
	else
		printf("RESET UNKNOW\n");

		IPCHandler *handler = CreateHandlerByStates();
		if (handler) {
			handler->run_parsing_command();
			delete handler;
			handler = NULL;
		} else {
			INFO("BUG@%d", __LINE__);
			sleep(1);
		}
#endif
	return 0;
}

IPCHandler *Watchdog::CreateHandlerByStates()
{
	switch (holder->GetMainStates()) {
		case POWERUP:
			fprintf(stdout, "%s: %s[%d] enter POWERUP\n", __FILE__, __FUNCTION__, __LINE__);
			return new PowerUpHandler();
			break;
		default:
			fprintf(stderr, "%s: %s[%d] BUG\n", __FILE__, __FUNCTION__, __LINE__);
			//return new PowerUpHandler();
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
	
#if 0
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
				dog->run();
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
#else
		if (!initSignal()) {
			ERROR("init signal handler failed.");
			return EXIT_FAILURE;
		}
		openlog("JOB4", LOG_PID | LOG_NDELAY, 0);
		do {
			dog = new Watchdog();
			if (dog) {
				dog->run();
				delete dog;
			} else {
				ERROR("can not alloc memory for dog");
				sleep(1);
			}

			sleep(5);
			printf("============================\n\n\n");
		} while (!get_quit());
		closelog();
		return EXIT_SUCCESS;
#endif
}


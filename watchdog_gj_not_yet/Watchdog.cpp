#include "Watchdog.h"
using namespace std;
Watchdog *dog = NULL;

Watchdog::Watchdog():hw_manager(NULL),holder(NULL)
{
	//TODO
	//make sure source is new or not?
	cout << "*** Jobber constructer without pra is running ... ***" << endl;

	hw_manager = HwManager::CreateHwManager();
	if (!hw_manager) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}

	holder = StatesHolder::CreateStatesHolder();
	if (!holder) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}

	handler = StatesHandler::CreateStatesHandler();	
	if (!handler) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

Watchdog::~Watchdog()
{
	//TODO
	//make sure source is free or not?
	cout << "*** Jobber destructer is running ... ***" << endl;
	
	if (handler) {
		StatesHandler::ReleaseStatesHandler();
		handler = NULL;
	}

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

	/* fprintf(stderr, "run LightSensorStatus detect thread\n"); */
	/* holder->run_status_detect_by_type("LightSensorStatus"); */

	fprintf(stderr, "run ButtonStatus detect thread\n");
	holder->run_status_detect_by_type("ButtonStatus");

	fprintf(stderr, "run LEDStatus detect thread\n");
	holder->run_status_detect_by_type("LEDStatus");

	fprintf(stderr, "run NetworkStatus detect thread\n");
	holder->run_status_detect_by_type("NetworkStatus");

	handler->run_parsing_command_by_main_states();

	return 0;
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

int main(int /*argc*/, const char* argv[])
{
	(void)argv; // Ignore parameters without "unused" warning
	
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


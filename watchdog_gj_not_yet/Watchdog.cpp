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
#define BUTTON false
#define WIFI false
#define LED false
#define IRLED false
#define ICR false
#define LIGHT_SENSOR false

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
#elif BUTTON

	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};

	printf("id is %d, status is %d\n", button_setting.id, button_setting.status);
	hw_manager->get_hw_info_by_type("BUTTON", &button_setting);
	
	if(button_setting.status == AHAL_BTN_STATUS_PRESSED)
		printf("RESET PRESSED!!!!!!!!!!!\n");
	else if (button_setting.status == AHAL_BTN_STATUS_RELEASE)
		printf("RESET RELEASE!!!!!!!!!!!\n");
	else
		printf("RESET UNKNOW\n");

#endif

#if 0
#elif WIFI

	/* RUN WPS */
	WIFI_SETTING wifi_setting {
		.DoWps = (AHAL_CST_BOOL) true
	};
	hw_manager->set_hw_info_by_type("WIFI", &wifi_setting);

	hw_manager->get_hw_info_by_type("WIFI", &wifi_setting);
	printf("rssi1 is %d\n", wifi_setting.wifi_radio_info.rssi1);

	/* Check WPS & Associated Status*/
	do {
		hw_manager->get_hw_info_by_type("WIFI", &wifi_setting);
		printf("wifi_wps_status is %d\n", wifi_setting.wifi_wps_status);

		if (wifi_setting.wifi_wps_status == AHAL_WIFI_WPS_STATUS_LINK_UP ||
			wifi_setting.wifi_wps_status == AHAL_WIFI_WPS_STATUS_IDLE	 ||
			(bool)wifi_setting.isAssociated == true) 
			break;

		sleep(1);
	} while(true);

	int count = 10;
	while(count) {
		hw_manager->get_hw_info_by_type("WIFI", &wifi_setting);
		if (wifi_setting.isAssociated == true) {
			printf("yes associated\n");
		} else
			printf("no associated\n");

		printf("rssi1 is %d\n", wifi_setting.wifi_radio_info.rssi1);
		
		count--;
		sleep(1);
	};

#endif

#if 0
#elif LED
	LED_SETTING led_setting {
		.id = AHAL_LED_ID_POWER,
		.color = AHAL_LED_COLOR_ON_A
	};

	hw_manager->set_hw_info_by_type("LED", &led_setting);
	sleep(1);
	led_setting.color = AHAL_LED_COLOR_ON_B;
	hw_manager->set_hw_info_by_type("LED", &led_setting);
	sleep(1);
	led_setting.color = AHAL_LED_COLOR_ON;
	hw_manager->set_hw_info_by_type("LED", &led_setting);
	sleep(1);
	led_setting.id = AHAL_LED_ID_WIFI_24G;
	led_setting.color = AHAL_LED_COLOR_ON_A;
	sleep(1);
	hw_manager->set_hw_info_by_type("LED", &led_setting);
	led_setting.color = AHAL_LED_COLOR_ON_B;
	sleep(1);
	hw_manager->set_hw_info_by_type("LED", &led_setting);
	led_setting.color = AHAL_LED_COLOR_ON;
	sleep(1);
	hw_manager->set_hw_info_by_type("LED", &led_setting);
#endif

#if 0
#elif IRLED
	IRLED_SETTING irled_setting {
		.id = AHAL_PWM_ID_IR_LED,
		.value_type = AHAL_PWM_PERCENT,
		.value = PWD_PERCENTAGE
	};
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	sleep(2);
	printf("80\n");
	irled_setting.value = 80;
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	sleep(2);
	printf("60\n");
	irled_setting.value = 60;
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	sleep(2);
	printf("40\n");
	irled_setting.value = 40;
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	sleep(2);
	printf("20\n");
	irled_setting.value = 20;
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	sleep(2);
	printf("0\n");
	irled_setting.value = 0;
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
#endif

#if 0
#elif ICR
	ICR_SETTING icr_setting {
		.state = AHAL_CST_STATE_ON			// NightMode
	};
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);
	sleep(5);
	icr_setting.state = AHAL_CST_STATE_OFF; // DayMode
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);
#endif

#if 0
#elif ICR
	ICR_SETTING icr_setting {
		.state = AHAL_CST_STATE_ON
	};
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);
	sleep(5);
	icr_setting.state = AHAL_CST_STATE_OFF;
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);
#endif

#if 0
#elif LIGHT_SENSOR
	LIGHT_SENSOR_SETTING light_sensor_setting {
		.value = 0
	};
	//FIXME just init and uninit again, or will occur i2c error
	for (int i = 0; i < 5; i++) {
		hw_manager->get_hw_info_by_type("LIGHTSENSOR", &light_sensor_setting);
		printf("LightSensor's lux is %d\n", light_sensor_setting.value);
		sleep(1);
	}
#endif

#if 1
	/*
	fprintf(stderr, "run ButtonStatus detect thread\n");
	holder->run_status_detect_by_type("ButtonStatus");

	fprintf(stderr, "run LightSensorStatus detect thread\n");
	holder->run_status_detect_by_type("LightSensorStatus");
	*/

	fprintf(stderr, "run LEDStatus detect thread\n");
	holder->run_status_detect_by_type("LEDStatus");

	fprintf(stderr, "run NetworkStatus detect thread\n");
	holder->run_status_detect_by_type("NetworkStatus");

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


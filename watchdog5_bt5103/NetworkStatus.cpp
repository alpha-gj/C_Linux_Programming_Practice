#include <iostream>
#include "NetworkStatus.h"

bool NetworkStatus::isPauseDetect = false;
NETWORK_STATUS_SETTING NetworkStatus::s_network_status_setting {
	.link_state = LINK_IS_OFF
};

NetworkStatus::NetworkStatus():network_status_pid(0)
{
	/* Do Nothing */
}

NetworkStatus::~NetworkStatus()
{
	if (network_status_pid) {
		pthread_join(network_status_pid, NULL);
	}
}

int NetworkStatus::init()
{
	fprintf(stderr, "NetworkStatus init\n");
	return (int)AHAL_RET_SUCCESS;
}

int NetworkStatus::deinit()
{
	fprintf(stderr, "NetworkStatus deinit\n");
	return (int)AHAL_RET_SUCCESS;
}

int NetworkStatus::run_status_detect()
{
	int ret = AHAL_RET_FAIL;

	do {
		ret = start_thread_join((void *) &run_network_status_thread, network_status_pid, hw_manager);
		if (ret != 0)
			break;

		ret = AHAL_RET_SUCCESS;
		printf("run_network_status_detect is OK\n");
	} while (false);

	return(int)ret;
}

int NetworkStatus::pause_status_detect() 
{
	isPauseDetect = true;
	return(int)AHAL_RET_SUCCESS;
}

int NetworkStatus::continue_status_detect()
{
	isPauseDetect = false;
	return(int)AHAL_RET_SUCCESS;
}

bool NetworkStatus::get_pause_detect_flag()
{
	return isPauseDetect;
}

void *NetworkStatus::run_network_status_thread(void *args)
{
	HwManager *pt_hw_manager = (HwManager *) args;
	WIFI_SETTING wifi_setting;
	SIGNAL_STAGE signal_stage = FIRST_STAGE;
	WIFI_LED_STATE wled_state = WLED_OFF;

	TinyDB db;
	db.init("Wireless");
	/* Call holder to set wled status according to wled_state */
	StatesHolder *holder = StatesHolder::CreateStatesHolder();

	while (!get_quit() && !get_reload()) {
		
		/* Pause detect condition */
		if (isPauseDetect) {
			sleep(1);
			continue;
		}

		/* Check Network State */
		do {
			/* Need to check enable Wireless or not first */
			if (db.getByte("Enable") == 1) {
				check_link_state();
			} else {
				break;
			}

			/* Wireless Enable isn't 1 or deassociated that should't do led state by rssi */
			if (s_network_status_setting.link_state == LINK_IS_OFF) {
				signal_stage = FIRST_STAGE;
				wled_state = WLED_WEAK;
				break;
			} else {
				/* Do Nothing */
			}

			/* Get wled_state and check wled needs to set again or not */
			wled_state = return_wled_state_by_rssi(wifi_setting.wifi_radio_info.rssi1, signal_stage);
			LED_STATUS_SETTING led_status_setting;

			holder->get_status_info_by_type("LEDStatus", &led_status_setting);
			switch (wled_state) {
				case WLED_OFF:
					led_status_setting.wled_state = WLED_OFF;
					break;
				case WLED_NORMAL: 
					led_status_setting.wled_state = WLED_NORMAL;
					break;
				case WLED_STRONG:
					led_status_setting.wled_state = WLED_STRONG;
					break;
				case WLED_WEAK:
				default:
					/* TODO: Daemon of associ_check will handle it */
					break;
			}
			holder->set_status_info_by_type("LEDStatus", &led_status_setting);

			/* Finsh first stage and then always do second stage until deassociated */
			if (signal_stage == FIRST_STAGE)
				signal_stage = SECOND_STAGE;

		} while (false);


		if (signal_stage == FIRST_STAGE)
			sleep(1);
		else if (signal_stage == SECOND_STAGE)
			sleep(5);
	};

	StatesHolder::ReleaseStatesHolder();
	holder = NULL;
	db.release();
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}

int NetworkStatus::set_status_info(void *) 
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int NetworkStatus::get_status_info(void * status_struct) 
{ 
	NETWORK_STATUS_SETTING *temp_network_status_setting = (NETWORK_STATUS_SETTING *) status_struct;

	temp_network_status_setting->link_state = s_network_status_setting.link_state;

	return(int)AHAL_RET_SUCCESS;
}

WIFI_LED_STATE NetworkStatus::return_wled_state_by_rssi(int rssi, SIGNAL_STAGE signal_stage)
{
	/* It should reset variable for second statge after finishing first stage */
	static bool needReset = true;		
	WIFI_LED_STATE wled_state = WLED_OFF;

	switch (signal_stage) {
		case FIRST_STAGE:
			wled_state = return_wled_state_from_first_stage(rssi);
			needReset = true;
			break;
		case SECOND_STAGE:
			wled_state = return_wled_state_from_second_stage(rssi, needReset);
			needReset = false;
			break;
		default:
			INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
			break;
	}

	return wled_state;
}

WIFI_LED_STATE NetworkStatus::return_wled_state_from_first_stage(int rssi)
{
	WIFI_LED_STATE wled_state = WLED_OFF;
	FILE *logfp = NULL;

	do {
		logfp = fopen("/dev/console", "w+");
		if (logfp == NULL) {
			printf("Open /dev/console fail: %s\n", strerror(errno));
			break;
		}

		fprintf(stderr, "Get Rx_SIGNAL(rssi1) is %d\n", rssi);
		/* Compare rssi with RSSI_UB_DEF to get wled_state */
		if (rssi >= RSSI_UB_DEF) {
			wled_state = WLED_STRONG;
			fprintf(logfp, YELLOW "Signal is strong at Stage 1" NONE "\n");
		} else {
			wled_state = WLED_NORMAL;
			fprintf(logfp, YELLOW "Signal is normal at Stage 2" NONE "\n");
		}

	} while (false);

	if (logfp != NULL)
		fclose(logfp);

	return wled_state;
}

WIFI_LED_STATE NetworkStatus::return_wled_state_from_second_stage(int rssi, bool isReset)
{
	WIFI_LED_STATE wled_state = WLED_OFF;
	static bool dontShowLog = false;
	static int n = 0;
	static unsigned int t = 0;
	static float array_rssi[10] = {}, mu = 0, sigma = 0;
	if (isReset == true) {
		/* Reset variable */
		dontShowLog = false;
		n = 0,
		t = 0;
		memset(&array_rssi, 0x00, sizeof(array_rssi));
		mu = 0, sigma = 0;
	}

	if (!dontShowLog) {
		FILE *logfp = NULL;

		do {
			logfp = fopen("/dev/console", "w+");
			if (logfp == NULL) {
				printf("Open /dev/console fail: %s\n", strerror(errno));
				break;
			}
			fprintf(logfp, YELLOW "Enter Stage 2" NONE "\n");
		} while (false);

		if (logfp != NULL)
			fclose(logfp);
		dontShowLog = true;
	}

	array_rssi[n] = rssi;
	t++;
	/*
	   test code
	   for(unsigned int a = 0; a < t && a < 10; a++) {
	   printf("rssi[%d] = %d\n", a, (int)rssi[a]);
	   }
	 */
	standard_deviation(array_rssi, t, &mu, &sigma);
	printf("Mu = %.2f \tSigma = %.2f\tVibration = %.2f\n", mu, sigma, mu - sigma);
	if (mu >= (RSSI_UB_DEF + 5) && (mu - sigma) >= RSSI_LB_DEF) {
		wled_state = WLED_STRONG;
		fprintf(stderr, "Signal is strong at Stage 2\n");
	} else {
		wled_state = WLED_NORMAL;
		fprintf(stderr, "Signal is normal at Stage 2\n");
	}
	n++;
	n %= 10;
	if (t == UINT_MAX) t = 10;

	return wled_state;
}

void NetworkStatus::standard_deviation(float data[], unsigned int t, float *mu, float *sigma)
{
	float mean=0.0, sum_deviation=0.0;
	int i;
	int n = (t > 10) ? 10:t;

	for (i = 0; i < n; ++i) 
		mean+=data[i];

	mean = mean/n;
	*mu = mean;

	for (i = 0; i < n; ++i)
		sum_deviation+=(data[i]-mean)*(data[i]-mean);

	*sigma = sqrt(sum_deviation/n);           
}

int NetworkStatus::check_link_state()
{
/* FIXME Need to check this project has ethernet or not */
#if 1
	IF_INFO if_info;
	if (does_wifi_associated(NULL) &&
			get_active_interface(PLATFORM_DEV_BR, &if_info) &&
			if_info.ipaddr.s_addr != 0) {	
		s_network_status_setting.link_state = LINK_IS_ON;
	} else {
		s_network_status_setting.link_state = LINK_IS_OFF;
	}	
	return 0;
#else 
	if (probe_link()) {
		return LINK_IS_ON;
	} else {
		IF_INFO if_info;
		if (does_wifi_associated(NULL) &&
				get_active_interface(PLATFORM_DEV_BR, &if_info) &&
				if_info.ipaddr.s_addr != 0) {
			return LINK_IS_ON;
		} else {
			return LINK_IS_OFF;
		}	
	}
#endif
}

int NetworkStatus::update_thread_value()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

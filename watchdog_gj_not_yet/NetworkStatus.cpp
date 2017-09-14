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
#if 1
	HwManager *pt_hw_manager = (HwManager *) args;
	WIFI_SETTING wifi_setting;
	SIGNAL_STAGE signal_stage = FIRST_STAGE;
	WIFI_LED_STATE wled_state = WLED_OFF;
	WIFI_LED_STATE old_wled_state = WLED_OFF;
	
	TinyDB db;
	db.init("Wireless");

	while (!get_quit() && !get_reload()) {
		
		/* Pause detect condition */
		if (isPauseDetect) {
			sleep(1);
			continue;
		}

		/* Check Network State */
		do {
			/* Wireless Enable isn't 1 or deassociated that should't do led state by rssi */
			pt_hw_manager->get_hw_info_by_type("WIFI", &wifi_setting);
			if (db.getByte("Enable") != 1 || wifi_setting.isAssociated != AHAL_CST_TRUE) {
				signal_stage = FIRST_STAGE;
				wled_state = WLED_WEAK;
				break;
			} else {
				/* Do Nothing */
			}

			/* Get wled_state */
			wled_state = return_wled_state_by_rssi(wifi_setting.wifi_radio_info.rssi1, signal_stage);
			
			/* Check it needs to set or not */
			if (wled_state == old_wled_state)
				break;
			else
				old_wled_state = wled_state;

			switch (wled_state) {
				case WLED_OFF:
					//SwStatus->LEDStatus
					break;
				case WLED_NORMAL:
					//SwStatus->LEDStatus
					break;
				case WLED_STRONG:
					//SwStatus->LEDStatus
					break;
				case WLED_WEAK:
				default:
					/* Daemon of associ_check will handle it */
					break;
			}

			/* Finsh first stage and then always do second stage until deassociated */
			if (signal_stage == FIRST_STAGE) {
				signal_stage = SECOND_STAGE;
			}

		} while (false);

		sleep(1);
	};

	db.release();
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
#endif
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
			fprintf(logfp, YELLOW "Signal is strong in Fisrt Stage" NONE "\n");
		} else {
			wled_state = WLED_NORMAL;
			fprintf(logfp, YELLOW "Signal is normal in Second Stage" NONE "\n");
		}

	} while (false);

	if (logfp != NULL)
		fclose(logfp);

	return wled_state;
}

WIFI_LED_STATE NetworkStatus::return_wled_state_from_second_stage(int rssi, bool isReset)
{
	WIFI_LED_STATE wled_state = WLED_OFF;
	//TODO
	return wled_state;
}

/* FIXME I think it need to put powerhandler */
LINK_STATE NetworkStatus::return_link_state()
{
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
}

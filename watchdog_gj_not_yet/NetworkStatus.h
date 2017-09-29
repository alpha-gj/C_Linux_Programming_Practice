#ifndef _NETWORKSTATUS_H_
#define _NETWORKSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <limits.h>
#include "Common.h"
#include "StatesHolder.h"
#include "SwStatus.h"
#include "LEDStatus.h"
#include "wlan/wlan.h"
#include "if_info/if_info.h"

using namespace std;

/* Follow spec. definition */
#define RSSI_UB_DEF			(-73)
#define RSSI_LB_DEF			(-85)

typedef enum {
	FIRST_STAGE,
	SECOND_STAGE,
} SIGNAL_STAGE;

typedef enum {
	LINK_IS_OFF,
	LINK_IS_ON,
} LINK_STATE;

/* TODO It should add other items for NEWTWORK_STATUS? */
typedef struct NETWORK_STATUS {
	LINK_STATE link_state;
} NETWORK_STATUS_SETTING;

class NetworkStatus : public SwStatus
{
	private:
		pthread_t network_status_pid;
		static void *run_network_status_thread(void *args);
		static bool isPauseDetect;
		static NETWORK_STATUS_SETTING s_network_status_setting;
		static int check_link_state();
		static WIFI_LED_STATE return_wled_state_by_rssi(int rssi, SIGNAL_STAGE signal_stage);
		static WIFI_LED_STATE return_wled_state_from_first_stage(int rssi);
		static WIFI_LED_STATE return_wled_state_from_second_stage(int rssi, bool isReset);
		static void standard_deviation(float data[], unsigned int t, float *mu, float *sigma);

	public:
		NetworkStatus();
		virtual ~NetworkStatus();
		virtual int init();
		virtual int deinit();
		virtual int run_status_detect();				
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual bool get_pause_detect_flag();
		virtual int set_status_info(void *status_struct = NULL);
		virtual int get_status_info(void *status_struct);
		virtual int update_thread_value();
};

#endif

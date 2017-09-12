#ifndef _LEDStatus_H_
#define _LEDStatus_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Common.h"
#include "SwStatus.h"
#include "LEDController.h"

using namespace std;

typedef enum {
	PLED_BOOTING,
	PLED_CLIENT_MODE,
	PLED_ACTIVE,
	PLED_WPS,
	PLED_FWUPDATE,
	PLED_AP_MODE,
	PLED_RESET,
	PLED_DISASSOCIATED,
	PLED_BT_MODE,
} POWER_LED_STATE;

typedef enum {
	WLED_OFF,
	WLED_WEAK,
	WLED_NORMAL,
	WLED_STRONG,
} WIFI_LED_STATE;

typedef struct LED_STATUS {
	POWER_LED_STATE pled_state;
	WIFI_LED_STATE wled_state;
} LED_STATUS_SETTING;

class LEDStatus : public SwStatus
{
	private:
		static void *run_led_status_thread(void *args);
		pthread_t led_status_pid;
		static bool isPauseDetect;
		static LED_STATUS_SETTING led_status_setting;

	public:
		LEDStatus();
		virtual ~LEDStatus();
		virtual int init();								
		virtual int deinit();
		virtual int run_status_detect();	
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual bool get_pause_detect_flag();
		virtual int set_status_info(void* status_struct);
		virtual int get_status_info(void* status_struct);
/* TODO
		void set_pled_state(POWER_LED_STATE p);
		POWER_LED_STATE get_pled_state();
		void set_wled_state(WPS_LED_STATE p);
		WPS_LED_STATE get_wled_state();
*/
};

#endif
	




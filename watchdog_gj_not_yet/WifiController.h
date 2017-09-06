#ifndef _WIFICONTROLLER_H_
#define _WIFICONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"
#include "wlan/wlan.h"

using namespace std;

typedef struct WIFI
{	
	/* RUN FUNCTION */
	AHAL_CST_BOOL DoWps;
	AHAL_CST_BOOL isAssociated;

	/* GET INFO */
	AHAL_WIFI_RADIO_INFO wifi_radio_info;
	AHAL_WIFI_WPS_STATUS wifi_wps_status;
} WIFI_SETTING;


class WifiController : public HwController
{
	private:
		int do_wifi_wps(void* hw_struct);
		int get_wifi_wps_status(void* hw_struct);
		int get_wifi_radio_status(void* hw_struct);
		int get_wifi_associated_status(void* hw_struct);

	public:
		WifiController();
		virtual ~WifiController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct);
		virtual	int get_hw_info(void* hw_struct);
};

#endif
	




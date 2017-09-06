#include <iostream>
#include "WifiController.h"

WifiController::WifiController()
{
	/* Do nothing */
}

WifiController::~WifiController()
{
	/* Do nothing */
}

int WifiController::init()
{
	fprintf(stderr, "WIFI init\n");
	return (int)AHAL_WIFI_Init();
}

int WifiController::deinit()
{
	fprintf(stderr, "WIFI deinit\n");
	return (int)AHAL_WIFI_UnInit();
}


int WifiController::set_hw_info(void *hw_struct) 
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;
	
	/* do wps */
	if (wifi_setting->DoWps &&
	    (ret = do_wifi_wps(hw_struct)) != AHAL_RET_SUCCESS) 		
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_WIFI_RunWPS fail\n");

	return (int)ret;
}

int WifiController::get_hw_info(void *hw_struct)
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;

	/* Get Wifi Associated */
	if ((ret = get_wifi_associated_status(hw_struct)) != AHAL_RET_SUCCESS)
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_WIFI_GetAssociated fail\n");

	/* Get Wifi WPS Status */
	if ((ret = get_wifi_wps_status(hw_struct)) != AHAL_RET_SUCCESS)
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_WIFI_GetWPSStatus fail\n");

	/* Get Wifi Radio Status */
	if (wifi_setting->isAssociated &&
		(ret = get_wifi_radio_status(hw_struct)) != AHAL_RET_SUCCESS) 		
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_WIFI_GetRadioStatus fail\n");

	return (int)ret;
}

int WifiController::do_wifi_wps(void *hw_struct)
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;
	
	ret = AHAL_WIFI_StartWPS(AHAL_WIFI_WPS_TYPE_ENROLLEE);
	wifi_setting->DoWps = (AHAL_CST_BOOL)false;

	return ret;
}

int WifiController::get_wifi_wps_status(void *hw_struct) 
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;

	ret = AHAL_WIFI_GetWPSStatus(AHAL_WIFI_WPS_TYPE_ENROLLEE,&wifi_setting->wifi_wps_status);

	return ret;
}

int WifiController::get_wifi_radio_status(void *hw_struct)
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;

	ret = AHAL_WIFI_GetRadioStatus(AHAL_WIFI_INTERFACE_24G,	&wifi_setting->wifi_radio_info);

	return ret;
}

int WifiController::get_wifi_associated_status(void *hw_struct)
{
	int ret = AHAL_RET_FAIL;
	WIFI_SETTING *wifi_setting = (WIFI_SETTING *) hw_struct;

	wifi_setting->isAssociated = (AHAL_CST_BOOL)does_wifi_associated(NULL);

	//TODO Always return SUCCESS
	ret = AHAL_RET_SUCCESS;

	return ret;
}

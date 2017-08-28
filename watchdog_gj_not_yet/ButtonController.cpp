#include "ButtonController.h"
#include <iostream>
using namespace std;

ButtonController::ButtonController()
{
	/* Do nothing */
}

~ButtonController::ButtonController()
{
	/* Do nothing */
}

int ButtonController::init()
{
	AHAL_RET ret = AHAL_BTN_Init();
	return (int)ret;
}

int ButtonController::deinit()
{
	AHAL_RET ret = AHAL_BTN_UnInit();
	return (int)ret;
}


int set_hw_info_pass_struct(void hw_struct) 
{
	AHAL_RET ret = AHAL_RET_NOT_SUPPORT;
	return (int)ret;
}

int run_hw_info_detect()
{
	/* TODO */
}

int get_hw_info_by_struct(void hw_struct)
{
	AHAL_RET ret = AHAL_RET_FAIL;
	BUTTON_SETTING button_setting =(BUTTON_SETTING) hw_struct;

	ret = AHAL_BTN_GetStatus(button_setting.id, &button_setting.status);
	if (ret != AHAL_RET_SUCCESS)
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_BTN_GetStatus fail\n");
	
	return (int)status;
}




#include <iostream>
#include "ButtonController.h"
using namespace std;

ButtonController::ButtonController()
{
	/* Do nothing */
}

ButtonController::~ButtonController()
{
	/* Do nothing */
}

int ButtonController::init()
{
	
	return (int)AHAL_BTN_Init();
}

int ButtonController::deinit()
{
	return (int)AHAL_BTN_UnInit();
}


int ButtonController::set_hw_info_by_struct(void *hw_struct) 
{
	return (int) AHAL_RET_NOT_SUPPORT;
}

int ButtonController::get_hw_info_by_struct(void *hw_struct)
{
	/*
	AHAL_RET ret = AHAL_RET_FAIL;
	BUTTON_SETTING* button_setting =(BUTTON_SETTING) *hw_struct;


	ret = AHAL_BTN_GetStatus(*(button_setting->id), *(button_setting->status));
	if (ret != AHAL_RET_SUCCESS)
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_BTN_GetStatus fail\n");
	
	return (int)button_setting.status;
	*/
	return -1;
}

int ButtonController::run_hw_info_detect()
{
	/* TODO */
	return AHAL_RET_NOT_SUPPORT;
}


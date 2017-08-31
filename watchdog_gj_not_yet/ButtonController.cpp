#include <iostream>
#include "ButtonController.h"

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
	fprintf(stderr, "Button init\n");
	return (int)AHAL_BTN_Init();
}

int ButtonController::deinit()
{
	fprintf(stderr, "Button deinit\n");
	return (int)AHAL_BTN_UnInit();
}


int ButtonController::set_hw_info(void *hw_struct) 
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

int ButtonController::get_hw_info(void *hw_struct)
{
	printf("get_hw_info\n");
	int ret = AHAL_RET_FAIL;
	BUTTON_SETTING *button_setting = (BUTTON_SETTING *) hw_struct;

	ret = AHAL_BTN_GetStatus(button_setting->id, &button_setting->status);
	if (ret != AHAL_RET_SUCCESS) {
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_BTN_GetStatus fail\n");
	}
	
	return (int)ret;
}

int ButtonController::run_hw_info_detect()
{
	/* TODO */
	return AHAL_RET_NOT_SUPPORT;
}


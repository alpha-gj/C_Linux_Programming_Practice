#include <iostream>
#include "LEDController.h"

LEDController::LEDController()
{
	/* Do nothing */
}

LEDController::~LEDController()
{
	/* Do nothing */
}

int LEDController::init()
{
	fprintf(stderr, "LED init\n");
	return (int)AHAL_LED_Init();
}

int LEDController::deinit()
{
	fprintf(stderr, "LED deinit\n");
	return (int)AHAL_LED_UnInit();
}


int LEDController::set_hw_info(void *hw_struct) 
{
	int ret = AHAL_RET_FAIL;
	LED_SETTING *led_setting = (LED_SETTING*) hw_struct;
	ret = AHAL_LED_SetState(led_setting->id, led_setting->color);
	if (ret != AHAL_RET_SUCCESS) {
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_LED_SetState %d fail\n", led_setting->id);
	}
	return (int)ret;
}

int LEDController::get_hw_info(void *)
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

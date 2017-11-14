#include <iostream>
#include "IRLEDController.h"

IRLEDController::IRLEDController()
{
	/* Do nothing */
}

IRLEDController::~IRLEDController()
{
	/* Do nothing */
}

int IRLEDController::init()
{
	fprintf(stderr, "PWM init\n");
	return (int)AHAL_PWM_Init();
}

int IRLEDController::deinit()
{
	fprintf(stderr, "PWM deinit\n");
	return (int)AHAL_PWM_UnInit();
}


int IRLEDController::set_hw_info(void *hw_struct) 
{
	int ret = AHAL_RET_FAIL;
	IRLED_SETTING *irled_setting = (IRLED_SETTING *) hw_struct;
	ret = AHAL_PWM_SetStrength(irled_setting->id, irled_setting->value_type, irled_setting->value);
	if (ret != AHAL_RET_SUCCESS) {
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_PWM_SetStrength %d fail\n", irled_setting->id);
	}
	return (int)ret;
}

int IRLEDController::get_hw_info(void *)
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

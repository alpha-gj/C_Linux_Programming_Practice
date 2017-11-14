#include <iostream>
#include "LightSensorController.h"

LightSensorController::LightSensorController()
{
	/* Do nothing */
}

LightSensorController::~LightSensorController()
{
	/* Do nothing */
}

int LightSensorController::init()
{
	fprintf(stderr, "LS init\n");
	return (int)AHAL_LS_Init();
}

int LightSensorController::deinit()
{
	fprintf(stderr, "LS deinit\n");
	return (int)AHAL_LS_UnInit();
}


int LightSensorController::set_hw_info(void *) 
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

int LightSensorController::get_hw_info(void *hw_struct)
{
	int ret = AHAL_RET_FAIL;

	LIGHT_SENSOR_SETTING *light_sensor_setting = (LIGHT_SENSOR_SETTING *) hw_struct;
	light_sensor_setting->value = AHAL_LS_GetLux();

	ret = AHAL_RET_SUCCESS;
	return (int)ret;
}

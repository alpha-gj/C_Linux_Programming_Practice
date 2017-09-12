#include <iostream>
#include "LightSensorStatus.h"

bool LightSensorStatus::isPauseDetect = false;

LightSensorStatus::LightSensorStatus():light_sensor_status_pid(0)
{
	/* Do Nothing */
}

LightSensorStatus::~LightSensorStatus()
{
	if (light_sensor_status_pid) {
		pthread_join(light_sensor_status_pid, NULL);
	}
}

int LightSensorStatus::init()
{
	fprintf(stderr, "LightSensorStatus init\n");
	return (int)AHAL_RET_SUCCESS;
}

int LightSensorStatus::deinit()
{
	fprintf(stderr, "LightSensorStatus deinit\n");
	return (int)AHAL_RET_SUCCESS;
}

int LightSensorStatus::run_status_detect()
{
	int ret = AHAL_RET_FAIL;

	do {
		ret = start_thread_join((void *) &run_light_sensor_status_detect_thread, light_sensor_status_pid, hw_manager);
		if (ret != 0)
			break;

		ret = AHAL_RET_SUCCESS;
		printf("run_status_detect is OK\n");
	} while(false);

	return(int)ret;
}

int LightSensorStatus::pause_status_detect() 
{
	isPauseDetect = true;
	return(int)AHAL_RET_SUCCESS;
}

int LightSensorStatus::continue_status_detect()
{
	isPauseDetect = false;
	return(int)AHAL_RET_SUCCESS;
}

bool LightSensorStatus::get_pause_detect_flag()
{
	return isPauseDetect;
}

void *LightSensorStatus::run_light_sensor_status_detect_thread(void *args)
{
	HwManager *pt_hw_manager = (HwManager *) args;
	LIGHT_SENSOR_SETTING light_sensor_setting {
		.value = 0
	};

	while (!get_quit() && !get_reload()) {
		
		/* Get LightSensor Status */
		pt_hw_manager->get_hw_info_by_type("LIGHTSENSOR", &light_sensor_setting);

		/* Pause detect condition */
		if (isPauseDetect) {
			printf("Pause LightSensorStatus\n");
			sleep(1);
			continue;
		} else {
			/* Do Nothing */
		}

		printf("LightSensor's lux is %d\n", light_sensor_setting.value);
		if (light_sensor_setting.value > 100) {
			printf("DayMode\n");
		} else if (light_sensor_setting.value < 90) 
			printf("NightMode\n");
		/* Check LightSensor Status and count it */

		/* Check DayMode or NightMode State */

		sleep(1);
	}
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}


int LightSensorStatus::set_status_info(void *)
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int LightSensorStatus::get_status_info(void *)
{
	return(int)AHAL_RET_NOT_SUPPORT;
}


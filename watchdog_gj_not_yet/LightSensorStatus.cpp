#include <iostream>
#include "LightSensorStatus.h"

bool LightSensorStatus::isPauseDetect = false;
LIGHT_SENSOR_STATUS_SETTING LightSensorStatus::s_light_sensor_status_setting {
	.ir_mode = IR_AUTO,
	.ir_led_state = IR_LED_OFF,
	.old_ir_led_state = IR_LED_unknown,
	.threshold = 0,
	.D2N_offset = 0,	 
	.N2D_offset = 0	
};

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

	/* 1st Step: Get light_sensor_settings from DB */
	LIGHT_SENSOR_SETTING_FROM_CAM light_sensor_setting_from_cam;

	if (cam_get_ir(&light_sensor_setting_from_cam.ir_set) != CAM_SUCCESS) {
		fprintf(stderr, "get ir set failed.\n");
		light_sensor_setting_from_cam.ir_set.ir_mode = 0; /* IR Auto */
	}
	if (cam_get_lightsensor(&light_sensor_setting_from_cam.lightsensor_set) != CAM_SUCCESS) {
		fprintf(stderr, "get light set failed.\n");
		light_sensor_setting_from_cam.ir_set.ir_mode = 0; /* IR Auto */
	}

	/* 2rd Step: Set IR LED STATE accroding to DB settings */
	s_light_sensor_status_setting.ir_led_state = get_IR_MODE_by_cam_settings(light_sensor_setting_from_cam);

	/* 3rd Step: Set IR MODE accroding to DB settings */
	if ((IR_MODE)light_sensor_setting_from_cam.ir_set.ir_mode == IR_AUTO)
		s_light_sensor_status_setting.ir_mode = IR_AUTO;
	else
		s_light_sensor_status_setting.ir_mode = IR_MANUAL;

	/* 4rd Step: Set IRLED/ICR behavior by state for IR Manual or not */
	if (s_light_sensor_status_setting.ir_mode != 0) { /* ir mode = 0 is IR Auto */
		set_ir_icr_behavior_by_state(s_light_sensor_status_setting.ir_led_state);
		s_light_sensor_status_setting.old_ir_led_state = s_light_sensor_status_setting.ir_led_state;
	}

	/* Set light_sensor_status_setting accroding to DB settings for IR Auto */
	s_light_sensor_status_setting.threshold = (int)light_sensor_setting_from_cam.lightsensor_set.light_sensor_level;
	if (s_light_sensor_status_setting.threshold <= MIN_threshold) {
		s_light_sensor_status_setting.threshold = MIN_threshold;
	}
	s_light_sensor_status_setting.D2N_offset = (int)light_sensor_setting_from_cam.lightsensor_set.D2N_offset;
	s_light_sensor_status_setting.N2D_offset = (int)light_sensor_setting_from_cam.lightsensor_set.N2D_offset;

	while (!get_quit() && !get_reload()) {

		/* Follow ir mode to do something */
		if (s_light_sensor_status_setting.ir_mode == IR_AUTO) {

			LIGHT_SENSOR_SETTING light_sensor_setting {
				.value = 0
			};
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
			printf("ir_led_state is %d\n", s_light_sensor_status_setting.ir_led_state);
			printf("old_ir_led_old_state is %d\n", s_light_sensor_status_setting.old_ir_led_state);

			/* Check LightSensor and light_sensor_status_setting.threshold */
			if (light_sensor_setting.value >= (s_light_sensor_status_setting.threshold + s_light_sensor_status_setting.N2D_offset)) {

				s_light_sensor_status_setting.ir_led_state = IR_LED_OFF;

			} else if (light_sensor_setting.value <= (s_light_sensor_status_setting.threshold - s_light_sensor_status_setting.D2N_offset)) {

				s_light_sensor_status_setting.ir_led_state = IR_LED_ON;

			} else {

				s_light_sensor_status_setting.ir_led_state = s_light_sensor_status_setting.old_ir_led_state;

			}

			/* Let's' count for it */
			static int count = 0;
			if (s_light_sensor_status_setting.old_ir_led_state != s_light_sensor_status_setting.ir_led_state) {
				++count;
				if (s_light_sensor_status_setting.old_ir_led_state == IR_LED_OFF) {
					if (count > DAY_TO_NIGHT_SEC) {
						INFO("Enter night mode");
						set_ir_icr_behavior_by_state(s_light_sensor_status_setting.ir_led_state);
						s_light_sensor_status_setting.old_ir_led_state = s_light_sensor_status_setting.ir_led_state;
						count = 0;
					}		
				} else  {							
					if (count > NIGHT_TO_DAY_SEC) {
						INFO("Enter day mode");
						set_ir_icr_behavior_by_state(s_light_sensor_status_setting.ir_led_state);
						s_light_sensor_status_setting.old_ir_led_state = s_light_sensor_status_setting.ir_led_state;
						count = 0;
					}		
				}
			} else {
				count = 0;
			}

			usleep(TIME_INTERVAL);
		} else if (s_light_sensor_status_setting.ir_mode == IR_MANUAL) {
			sleep(2);
		} else {
			/* Do Nothing */
		}
	}
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}


int LightSensorStatus::set_status_info(void *status_struct)
{
	LIGHT_SENSOR_STATUS_SETTING *temp_light_sensor_status_setting = (LIGHT_SENSOR_STATUS_SETTING *) status_struct;
	TinyDB db;

	db.init("Image", true);

	if (temp_light_sensor_status_setting->ir_mode == IR_AUTO) {
		db.setByte("IrAuto", 1); /* IrAuto is 1 for auto, but def. from cam, ir_mode = 0 when set auto */
		s_light_sensor_status_setting.ir_mode = IR_AUTO;
	} else if (temp_light_sensor_status_setting->ir_mode == IR_MANUAL) {
		db.setByte("IrAuto", 0); /* IrAuto is 1 for auto, but def. from cam, ir_mode = 0 when set auto */
		s_light_sensor_status_setting.ir_mode = IR_MANUAL;
	}

	s_light_sensor_status_setting.ir_led_state		= temp_light_sensor_status_setting->ir_led_state;
	s_light_sensor_status_setting.old_ir_led_state	= temp_light_sensor_status_setting->old_ir_led_state;
	s_light_sensor_status_setting.threshold			= temp_light_sensor_status_setting->threshold;
	s_light_sensor_status_setting.D2N_offset		= temp_light_sensor_status_setting->D2N_offset;
	s_light_sensor_status_setting.N2D_offset		= temp_light_sensor_status_setting->N2D_offset;

	db.release();
	return (int)AHAL_RET_SUCCESS;
}

int LightSensorStatus::get_status_info(void *status_struct)
{
	LIGHT_SENSOR_STATUS_SETTING *temp_light_sensor_status_setting = (LIGHT_SENSOR_STATUS_SETTING *) status_struct;

	temp_light_sensor_status_setting->ir_mode		   = s_light_sensor_status_setting.ir_mode;
	temp_light_sensor_status_setting->ir_led_state	   = s_light_sensor_status_setting.ir_led_state;
	temp_light_sensor_status_setting->old_ir_led_state = s_light_sensor_status_setting.old_ir_led_state;
	temp_light_sensor_status_setting->threshold		   = s_light_sensor_status_setting.threshold;
	temp_light_sensor_status_setting->D2N_offset	   = s_light_sensor_status_setting.D2N_offset;
	temp_light_sensor_status_setting->N2D_offset	   = s_light_sensor_status_setting.N2D_offset;

	return (int)AHAL_RET_SUCCESS;
}

IR_LED_STATE LightSensorStatus::get_IR_MODE_by_cam_settings(LIGHT_SENSOR_SETTING_FROM_CAM light_sensor_setting_from_cam) 
{
	if (light_sensor_setting_from_cam.ir_set.ir_mode == 3) {		   /* Always NightMode, def. from cam.h */
		printf("IR_LED_ON\n");
		return IR_LED_ON;
		
	} else if(light_sensor_setting_from_cam.ir_set.ir_mode  == 2) {	   /* Always DayMode, def. from cam.h */
		printf("IR_LED_OFF\n");
		return IR_LED_OFF;
	} else {
		printf("IR_LED_STATE follow cam settings\n");
		return (IR_LED_STATE)light_sensor_setting_from_cam.ir_set.ir_state;
	}
}

int LightSensorStatus::set_ir_icr_behavior_by_state(IR_LED_STATE ir_led_state) 
{
	if (ir_led_state == IR_LED_OFF) {
		send_ipccmd(WATCHDOG_SOCKET_NAME, CMD_ENTER_DAY_MODE);
	} else if (ir_led_state == IR_LED_ON) {
		send_ipccmd(WATCHDOG_SOCKET_NAME, CMD_ENTER_NIGHT_MODE);
	} else {
		/* Do Nothing */
	}
}

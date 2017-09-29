#include <iostream>
#include "LEDStatus.h"

bool LEDStatus::isPauseDetect = false;
LED_STATUS_SETTING LEDStatus::s_led_status_setting {
	.pled_state = PLED_BOOTING,
	.wled_state = WLED_OFF,
	.enable_silent_led = false
};

LEDStatus::LEDStatus():led_status_pid(0)
{
	/* Do Nothing */
}

LEDStatus::~LEDStatus()
{
	if (led_status_pid) {
		pthread_join(led_status_pid, NULL);
	}
}

int LEDStatus::init()
{
	fprintf(stderr, "LEDStatus init\n");
	return (int)AHAL_RET_SUCCESS;
}

int LEDStatus::deinit()
{
	fprintf(stderr, "LEDStatus deinit\n");
	return (int)AHAL_RET_SUCCESS;
}

int LEDStatus::run_status_detect()
{
	int ret = AHAL_RET_FAIL;

	do {
		ret = start_thread_join((void *) &run_led_status_thread, led_status_pid, hw_manager);
		if (ret != 0)
			break;

		ret = AHAL_RET_SUCCESS;
		printf("run_led_status_detect is OK\n");
	} while(false);

	return(int)ret;
}

int LEDStatus::pause_status_detect() 
{
	isPauseDetect = true;
	return(int)AHAL_RET_SUCCESS;
}

int LEDStatus::continue_status_detect()
{
	isPauseDetect = false;
	return(int)AHAL_RET_SUCCESS;
}

bool LEDStatus::get_pause_detect_flag()
{
	return isPauseDetect;
}

void *LEDStatus::run_led_status_thread(void *args)
{
	HwManager *pt_hw_manager = (HwManager *) args;
	LED_SETTING_FROM_CAM led_setting_from_cam;
	LED_SETTING led_setting;

	if (cam_get_gpio(&led_setting_from_cam.gpio_set) != CAM_SUCCESS) {
		fprintf(stderr, "get gpio setting failed.\n");
		led_setting_from_cam.gpio_set.silent_led = 0;
	}

	/* Set SilentLED or not */
	if (led_setting_from_cam.gpio_set.silent_led == 0)
		s_led_status_setting.enable_silent_led = false;
	else
		s_led_status_setting.enable_silent_led = true;

	while (!get_quit() && !get_reload()) {

		/* Pause detect condition */
		if (isPauseDetect) {
			sleep(1);
			continue;
		}

		/* 
			NOTE: Don't use old_state and state to decide to set LED status again or not.
			It wouldn't do LED Blinking effect for some LED states.
    	*/
		/* Set LED State */
		set_pled_status_by_state(s_led_status_setting.pled_state, &led_setting);
		pt_hw_manager->set_hw_info_by_type("LED", &led_setting);
		//printf("led_setting.id is %d, led_setting.color is %d\n", led_setting.id, led_setting.color);

		set_wled_status_by_state(s_led_status_setting.wled_state, &led_setting);
		pt_hw_manager->set_hw_info_by_type("LED", &led_setting);
		//printf("led_setting.id is %d, led_setting.color is %d\n", led_setting.id, led_setting.color);

		sleep(1);
	}

	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}

int LEDStatus::set_status_info(void* status_struct) 
{
	/* TODO It should do pthread_mux_lock? */
	LED_STATUS_SETTING *temp_led_status_setting = (LED_STATUS_SETTING *) status_struct;

	s_led_status_setting.pled_state = temp_led_status_setting->pled_state;
	s_led_status_setting.wled_state = temp_led_status_setting->wled_state;

	return(int)AHAL_RET_SUCCESS;
}

int LEDStatus::get_status_info(void* status_struct)
{
	/* TODO It should do pthread_mux_lock? */
	LED_STATUS_SETTING *temp_led_status_setting = (LED_STATUS_SETTING *) status_struct;

	temp_led_status_setting->pled_state = s_led_status_setting.pled_state;
	temp_led_status_setting->wled_state = s_led_status_setting.wled_state;

	return(int)AHAL_RET_SUCCESS;
}

int LEDStatus::set_pled_status_by_state(POWER_LED_STATE pled_state, LED_SETTING *led_setting)
{
	static bool pled_on = false;
	pled_on = !pled_on;

	switch (pled_state) {
		case PLED_BOOTING:
		case PLED_FWUPDATE:
		case PLED_DISASSOCIATED:
			/* Solid Red */
			led_setting->id = AHAL_LED_ID_POWER;
			led_setting->color = AHAL_LED_COLOR_ON_A;
			break;
		case PLED_BT_MODE:
		case PLED_AP_MODE:
			/* Blinking Orange */
			if(pled_on) {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_ON;
			} else {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_OFF;
			}
			break;
		case PLED_CLIENT_MODE:
			/* Solid Green */
			if (get_need_silent_led_status()) {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_OFF;
			} else {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_ON_B;
			}
			break;
		case PLED_WPS:
		case PLED_ACTIVE:
			/* Blinking Green */
			if (get_need_silent_led_status()) {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_OFF;
				break;
			} 
			if(pled_on) {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_ON_B;
			} else {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_OFF;
			}
			break;
		case PLED_RESET:
			/* Blinking Red */
			if(pled_on) {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_ON_A;
			} else {
				led_setting->id = AHAL_LED_ID_POWER;
				led_setting->color = AHAL_LED_COLOR_OFF;
			}
			break;
		default:
			INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
			break;
	}

	return 0;
}

int LEDStatus::set_wled_status_by_state(WIFI_LED_STATE wled_state, LED_SETTING *led_setting)
{
	static bool wled_on = false;
	wled_on = !wled_on;

	/* Check it needs to silent led or not */
	if (get_need_silent_led_status())
		wled_state = WLED_OFF;

	switch(wled_state) {
		case WLED_OFF:
			led_setting->id = AHAL_LED_ID_WIFI_24G;
			led_setting->color = AHAL_LED_COLOR_OFF;
			break;
		case WLED_WEAK:
			/* Blinking Orange */
			if(wled_on) {
				led_setting->id = AHAL_LED_ID_WIFI_24G;
				led_setting->color = AHAL_LED_COLOR_ON;
			} else {
				led_setting->id = AHAL_LED_ID_WIFI_24G;
				led_setting->color = AHAL_LED_COLOR_OFF;
			}
			break;
		case WLED_NORMAL:
			/* Solid Orange */
			led_setting->id = AHAL_LED_ID_WIFI_24G;
			led_setting->color = AHAL_LED_COLOR_ON;
			break;
		case WLED_STRONG:
			/* Solid Green */
			led_setting->id = AHAL_LED_ID_WIFI_24G;
			led_setting->color = AHAL_LED_COLOR_ON_B;
			break;
		default:
			break;
	}

	return 0;
}

bool LEDStatus::get_need_silent_led_status()
{
	return s_led_status_setting.enable_silent_led;
}

int LEDStatus::update_thread_value()
{
	LED_SETTING_FROM_CAM led_setting_from_cam;

	if (cam_get_gpio(&led_setting_from_cam.gpio_set) != CAM_SUCCESS) {
		fprintf(stderr, "get gpio setting failed.\n");
		led_setting_from_cam.gpio_set.silent_led = 0;
	}

	if (led_setting_from_cam.gpio_set.silent_led == 0)
		s_led_status_setting.enable_silent_led = false;
	else
		s_led_status_setting.enable_silent_led = true;

	return 0;
}

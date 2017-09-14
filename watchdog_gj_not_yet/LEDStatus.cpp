#include <iostream>
#include "LEDStatus.h"

bool LEDStatus::isPauseDetect = false;
LED_STATUS_SETTING LEDStatus::s_led_status_setting {
	.pled_state = PLED_BOOTING,
	.wled_state = WLED_OFF
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
#if 1
	HwManager *pt_hw_manager = (HwManager *) args;
	LED_SETTING led_setting;
	bool pled_on = false;

	while (!get_quit() && !get_reload()) {
		//FIXME Debug message
		printf("led_setting.id is %d, led_setting.color is %d\n", led_setting.id, led_setting.color);
		
		/* Pause detect condition */
		if (isPauseDetect) {
			sleep(1);
			continue;
		}

		/* Check LED State */
		pled_on = !pled_on;
		switch (s_led_status_setting.pled_state) {
			case PLED_BOOTING:
			case PLED_FWUPDATE:
			case PLED_DISASSOCIATED:
				/* Solid Red */
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_ON_A;
				break;
			case PLED_RESET:
				/* Blinking Red */
				if(pled_on) {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_ON_A;
				} else {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_OFF;
				}
				break;
			case PLED_CLIENT_MODE:
				/* Solid Green */
				if(false) { //FIXME
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_OFF;
				} else {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_ON_B;
				}
				break;
			case PLED_WPS:
			case PLED_ACTIVE:
				/* Blinking Green */
				if(false) { //FIXME
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_OFF;
					break;
				} 
				if(pled_on) {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_ON_B;
				} else {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_OFF;
				}
				break;
			case PLED_BT_MODE:
			case PLED_AP_MODE:
				/* Blinking Orange */
				if(pled_on) {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_ON;
				} else {
					led_setting.id = AHAL_LED_ID_POWER;
					led_setting.color = AHAL_LED_COLOR_OFF;
				}
				break;
			default:
				INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
				break;
		}

		pt_hw_manager->set_hw_info_by_type("LED", &led_setting);

		sleep(1);
	}

	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
#endif
}

int LEDStatus::set_status_info(void* status_struct) 
{
	LED_STATUS_SETTING *temp_led_status_setting = (LED_STATUS_SETTING *) status_struct;

	s_led_status_setting.pled_state = temp_led_status_setting->pled_state;
	s_led_status_setting.wled_state = temp_led_status_setting->wled_state;

	return(int)AHAL_RET_SUCCESS;
}

int LEDStatus::get_status_info(void* status_struct)
{
	LED_STATUS_SETTING *temp_led_status_setting = (LED_STATUS_SETTING *) status_struct;

	temp_led_status_setting->pled_state = s_led_status_setting.pled_state;
	temp_led_status_setting->wled_state = s_led_status_setting.wled_state;

	return(int)AHAL_RET_SUCCESS;
}

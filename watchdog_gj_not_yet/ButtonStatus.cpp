#include <iostream>
#include "ButtonStatus.h"

bool ButtonStatus::isPauseDetect = false;

ButtonStatus::ButtonStatus():reset_button_status_pid(0)
{
	/* Do Nothing */
}

ButtonStatus::~ButtonStatus()
{
	if (reset_button_status_pid) {
		pthread_join(reset_button_status_pid, NULL);
	}
}

int ButtonStatus::init()
{
	fprintf(stderr, "ButtonStatus init\n");
	return (int)AHAL_RET_SUCCESS;
}

int ButtonStatus::deinit()
{
	fprintf(stderr, "ButtonStatus deinit\n");
	return (int)AHAL_RET_SUCCESS;
}

int ButtonStatus::run_status_detect()
{
	int ret = AHAL_RET_FAIL;

	do {
		ret = start_thread_join((void *) &run_reset_button_status_detect_thread, reset_button_status_pid, hw_manager);
		if (ret != 0)
			break;

		ret = AHAL_RET_SUCCESS;
		printf("run_status_detect is OK\n");
	} while(false);

	return(int)ret;
}

int ButtonStatus::pause_status_detect() 
{
	isPauseDetect = true;
	return(int)AHAL_RET_SUCCESS;
}

int ButtonStatus::continue_status_detect()
{
	isPauseDetect = false;
	return(int)AHAL_RET_SUCCESS;
}

bool ButtonStatus::get_pause_detect_flag()
{
	return isPauseDetect;
}

void *ButtonStatus::run_reset_button_status_detect_thread(void *args)
{
	HwManager *pt_hw_manager = (HwManager *) args;
	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};
	RESET_BUTTON_STATE state = RESET_READY;
	unsigned int press_count = 0;

	while (!get_quit() && !get_reload()) {
		
		/* Get Button Status */
		pt_hw_manager->get_hw_info_by_type("BUTTON", &button_setting);

		/* Pause detect condition */
		if (isPauseDetect ||
		    (state == RESET_LAUNCH && button_setting.status == AHAL_BTN_STATUS_PRESSED)) {
			printf("continue\n");
			sleep(1);
			continue;
		} else {
			/* Do Nothing */
		}

		/* Check Button Status and count it */
		if (button_setting.status == AHAL_BTN_STATUS_PRESSED) {
			press_count++;
		} else if (button_setting.status == AHAL_BTN_STATUS_RELEASE) {
			press_count = 0;
		} else if (button_setting.status == AHAL_BTN_STATUS_UNKNOWN) {
			ERROR("RESET BUTTON Get AHAL_BTN_STATUS_UNKNOWN");
			press_count = 0;
		}
		else {
			/* Do Nothing */
		}

		/* Check Button State */
		state = return_button_state_by_press_count(press_count);
		switch (state) {
			case RESET_READY:
				break;
			case RESET_STANDBY:
			case RESET_LAUNCH:
				INFO("Enter RESET");
				/*
				   TODO Task should belong to xxxHandler.cpp
				   dog->set_reset_behavior_by_state(state);
				 */
				break;
			default:
				INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
				break;
		}

		sleep(1);
	}
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}

RESET_BUTTON_STATE ButtonStatus::return_button_state_by_press_count(int press_count)
{
	PIB pib;
	if (press_count >= pib.get_reset_button_press_count()) {
		return RESET_LAUNCH;
	}
	else {
		return RESET_READY;
	}
}

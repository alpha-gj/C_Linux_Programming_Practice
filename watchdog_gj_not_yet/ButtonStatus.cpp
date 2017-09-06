#include <iostream>
#include "ButtonStatus.h"

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
	return(int)AHAL_RET_NOT_SUPPORT;
}

int ButtonStatus::continue_status_detect()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int ButtonStatus::get_status_detect_flag()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

void *ButtonStatus::run_reset_button_status_detect_thread(void *args)
{
	//unsigned int press_count = 0;
	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};

	//PIB pib;
	//RESET_BUTTON_STATE state = RESET_READY;
	/*
		TODO Task should belong to xxxHandler.cpp
		dog->set_reset_behavior_by_state(state);
	*/

#if 1
	HwManager *pt_hw_manager = (HwManager *) args;
	while (!get_quit() && !get_reload()) {

		printf("hello!\n");

		pt_hw_manager->get_hw_info_by_type("BUTTON", &button_setting);

		if(button_setting.status == AHAL_BTN_STATUS_PRESSED)
			printf("RESET PRESSED!!!!!!!!!!!\n");
		else if (button_setting.status == AHAL_BTN_STATUS_RELEASE)
			printf("RESET RELEASE!!!!!!!!!!!\n");
		else
			printf("RESET UNKNOW\n");

		sleep(1);
	}
#else
	while (!is_reload && !is_quit) {
		on = p->ctrl->get_reset_button();
		switch (state) {
		case RESET_READY:
			if (on) {
				++press_count;
				if (press_count > pib.get_reset_button_press_count()) {
					state = RESET_STANDBY;
					dog->set_reset_behavior_by_state(state);
					press_count = 0;
				}
			} else {
				press_count = 0;
			}
			break;
		case RESET_STANDBY:
			if (on) {
				// do nothing
			} else {
				state = RESET_LAUNCH;
				dog->set_reset_behavior_by_state(state);
			}
			break;
		case RESET_LAUNCH:
			// do nothing, system should do factory reset
			break;
		}
		sleep(1);
	}
#endif
	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}


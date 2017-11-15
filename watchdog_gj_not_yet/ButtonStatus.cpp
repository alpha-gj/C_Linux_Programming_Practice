#include "ButtonStatus.h"
#include "StatesHandler.h"

bool ButtonStatus::isPauseDetect = false;
StatesHandler *ButtonStatus::handler = NULL;


ButtonStatus::ButtonStatus():reset_button_status_pid(0)
{
	BUTTON_STATE button_state {
		.reset_button_state = RESET_LAUNCH
	};
	/* pthread_mutex_init */
	pthread_mutex_init(&SetResetButtonStatesLock, NULL);
	pthread_mutex_init(&GetResetButtonStatesLock, NULL);
}

ButtonStatus::~ButtonStatus()
{
	/* pthread_mutex_destroy */
	pthread_mutex_destroy(&SetResetButtonStatesLock);
	pthread_mutex_destroy(&GetResetButtonStatesLock);

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
	handler = StatesHandler::CreateStatesHandler();	

	while (!get_quit() && !get_reload()) {
		
		/* Pause detect condition */
		if (isPauseDetect) {
			sleep(1);
			continue;
		} 

		if (handler != NULL) {
			handler->do_event_by_main_states(BUTTON_EVENT);
		} 

		sleep(1);
	}

	StatesHandler::ReleaseStatesHandler();	
	handler = NULL;

	fprintf(stderr, "%s done\n", __func__);
	pthread_exit(0);
}

int ButtonStatus::set_status_info(void *status_struct) 
{	
	pthread_mutex_lock(&SetResetButtonStatesLock);
	BUTTON_STATE *t_button_state = (BUTTON_STATE *) status_struct;
	button_state.reset_button_state = t_button_state->reset_button_state;
	pthread_mutex_unlock(&SetResetButtonStatesLock);
	return 0;
}
int ButtonStatus::get_status_info(void *status_struct)
{
	pthread_mutex_lock(&GetResetButtonStatesLock);
	BUTTON_STATE *t_button_state = (BUTTON_STATE *) status_struct;
	t_button_state->reset_button_state = button_state.reset_button_state;
	pthread_mutex_unlock(&GetResetButtonStatesLock);
	return 0;
}


int ButtonStatus::update_thread_value()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

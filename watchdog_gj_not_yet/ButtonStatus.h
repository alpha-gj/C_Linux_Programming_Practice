#ifndef _BUTTONSTATUS_H_
#define _BUTTONSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "Common.h"
#include "SwStatus.h"
#include "ButtonController.h"

using namespace std;

typedef enum {
	RESET_READY,
	RESET_STANDBY,
	RESET_LAUNCH,
} RESET_BUTTON_STATE;

typedef  struct BUTTON_STATE {
	RESET_BUTTON_STATE reset_button_state;	
} BUTTIN_STATE;

class StatesHandler;
class ButtonStatus : public SwStatus
{
	private:
		/* For detect thread */
		static void *run_reset_button_status_detect_thread(void *args);
		pthread_t reset_button_status_pid;
		static bool isPauseDetect;
		static StatesHandler *handler;

		/* For status info */
		pthread_mutex_t SetResetButtonStatesLock;
		pthread_mutex_t GetResetButtonStatesLock;
		BUTTON_STATE button_state;

	public:
		ButtonStatus();
		virtual ~ButtonStatus();
		virtual int init();								
		virtual int deinit();
		virtual int run_status_detect();	
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual bool get_pause_detect_flag();
		virtual int set_status_info(void* status_struct);
		virtual int get_status_info(void* status_struct);
		virtual int update_thread_value();
};

#endif
	




#ifndef _BUTTONSTATUS_H_
#define _BUTTONSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Common.h"
#include "SwStatus.h"
#include "ButtonController.h"

using namespace std;

typedef enum {
	RESET_READY,
	RESET_STANDBY,
	RESET_LAUNCH,
} RESET_BUTTON_STATE;

class ButtonStatus : public SwStatus
{
	private:
		static void *run_reset_button_status_detect_thread(void *args);
		pthread_t reset_button_status_pid;
		static bool isPauseDetect;
		static RESET_BUTTON_STATE return_button_state_by_press_count(int press_count);

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
	




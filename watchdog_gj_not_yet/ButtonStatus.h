#ifndef _BUTTONSTATUS_H_
#define _BUTTONSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Common.h"
#include "SwStatus.h"
#include "ButtonController.h"

using namespace std;

class ButtonStatus : public SwStatus
{
	private:
		pthread_t reset_button_status_pid;
		static void *run_reset_button_status_detect_thread(void *args = NULL);

	public:
		ButtonStatus();
		virtual ~ButtonStatus();
		virtual int init();								/* To check or not according to DB value  */
		virtual int deinit();
		virtual int run_status_detect();				/* TODO Need pthread?  Pass struct of data type for checking HW info */
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual int get_status_detect_flag();
};

#endif
	




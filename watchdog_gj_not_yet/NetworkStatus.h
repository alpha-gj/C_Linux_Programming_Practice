#ifndef _NETWORKSTATUS_H_
#define _NETWORKSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "SwStatus.h"

using namespace std;

class NetworkStatus : public SwStatus
{
	public:
		NetworkStatus();
		virtual ~NetworkStatus();
		virtual int init();								/* To check or not according to DB value  */
		virtual int deinit();
		virtual int run_status_detect();				/* TODO Need pthread?  Pass struct of data type for checking HW info */
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual int get_status_detect_flag();
};

#endif
	




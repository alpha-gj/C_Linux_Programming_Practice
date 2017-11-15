#ifndef _STATESHANDLER_H_
#define _STATESHANDLER_H_
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "StatesHolder.h"
#include "IPCHandler.h"
#include "BootingHandler.h"
#include "FactoryResetHandler.h"

class StatesHandler
{
	private:
		/* Deny constructor & copy constructor by externel */
		StatesHandler();
		~StatesHandler();
		StatesHandler(const StatesHandler&);
		StatesHandler& operator=(const StatesHandler&);

		/* Follow Singleton Pattern */
		static StatesHandler *handler;
		static int referCount;
	
		/* Common variable */
		StatesHolder *holder; 

		/* Access states object from function */
		IPCHandler *ReturnMainIPCHandlerObject(MAINSTATES states);

	public:
		/* Follow Singleton Pattern */
		static StatesHandler *CreateStatesHandler();
		static int ReleaseStatesHandler();

		/* Operate IPCHandler object from API */

		/* For init & deinit IPCHandler objects */
		int init();
		int deinit();

		int run_parsing_command_by_main_states();
		int do_event_by_main_states(EVENT sw_event_name);
};

#endif

#ifndef _STATESHOLDER_H_
#define _STATESHOLDER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "SwStatus.h"
#include "NetworkStatus.h"
#include "ButtonStatus.h"
#include "LightSensorStatus.h"
#include "LEDStatus.h"

//TODO They can include State.h? 
enum MAINSTATES {
	POWERUP,
	MAINSTATES_COUNT,
};

class StatesHolder
{
	private:
		/* Deny constructor & copy constructor by externel */
		StatesHolder();
		~StatesHolder();
		StatesHolder(const StatesHolder&);
		StatesHolder& operator=(const StatesHolder&);

		/* Follow Singleton Pattern */
		static StatesHolder *holder;
		static int referCount;

		/* For init & deinit SwStatus objects */
		int init();
		int deinit();

		/* 
			NOTE: Don't use const char* as primary key of STL map.
			It doesn't comapre "xxx", rather it comapres 0x12345678 
			by using find(...) of STL map. Please use string to replace it.
    	*/
		/* Access Status object from MAP */
		map<string, SwStatus *> map_sw_status;
		SwStatus *ReturnSwStatusObjectByType(const char* sw_status_name);

		//pthread_mutex_t mainStatesLock;
		MAINSTATES mainStates;
		MAINSTATES oldMainStates;

	public:
		/* Follow Singleton Pattern */
		static StatesHolder *CreateStatesHolder();
		static int ReleaseStatesHolder();
		
		/* Operate SwStatus object from API */
		int init_status_detect_by_type(const char* status_name);
		int deinit_status_detect_by_type(const char* status_name);
		int run_status_detect_by_type(const char* status_name);
		int pause_status_detect_by_type(const char* status_name);
		int continue_status_detect_by_type(const char* status_name);
		int get_pause_detect_flag_by_type(const char* status_name);
		int set_status_info_by_type(const char* status_name, void* status_struct);
		int get_status_info_by_type(const char* status_name, void* status_struct);

		void SetMainStates(MAINSTATES s);
		MAINSTATES GetMainStates();
		MAINSTATES GetOldMainStates();
};

#endif

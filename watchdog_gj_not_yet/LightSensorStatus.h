#ifndef _LIGHTSENSORSTATUS_H_
#define _LIGHTSENSORSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Common.h"
#include "SwStatus.h"
#include "LightSensorController.h"

using namespace std;

typedef enum {
	LIGHT_OFF,
	LIGHT_ON,
} LIGHT_SENSOR_STATE;

class LightSensorStatus : public SwStatus
{
	private:
		pthread_t light_sensor_status_pid;
		static void *run_light_sensor_status_detect_thread(void *args);
		static bool isPauseDetect;

	public:
		LightSensorStatus();
		virtual ~LightSensorStatus();
		virtual int init();								
		virtual int deinit();
		virtual int run_status_detect();	
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual bool get_pause_detect_flag();
		virtual int set_status_info(void *status_struct = NULL);
		virtual int get_status_info(void *status_struct = NULL);
};

#endif
	




#ifndef _LIGHTSENSORCONTROLLER_H_
#define _LIGHTSENSORCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

/* TODO HW define parameters */
#define TIME_INTERVAL		500000
#define DAY_TO_NIGHT_SEC	3 * (1000000/TIME_INTERVAL)
#define NIGHT_TO_DAY_SEC	7 * (1000000/TIME_INTERVAL)

typedef struct LIGHT_SENSOR
{	
	AHAL_INT32 value;
} LIGHT_SENSOR_SETTING;

class LightSensorController : public HwController
{
	public:
		LightSensorController();
		virtual ~LightSensorController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct);
		virtual	int get_hw_info(void* hw_struct = NULL);
};
#endif
	




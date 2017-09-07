#ifndef _LIGHTSENSORCONTROLLER_H_
#define _LIGHTSENSORCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

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
	




#ifndef _IRLEDCONTROLLER_H_
#define _IRLEDCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

#define PWD_PERCENTAGE 100

typedef struct IRLED
{	
	AHAL_PWM_ID id;
	AHAL_PWM_VALUE_TYPE value_type; 
	AHAL_INT32 value;
} IRLED_SETTING;

class IRLEDController : public HwController
{
	public:
		IRLEDController();
		virtual ~IRLEDController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct);
		virtual	int get_hw_info(void* hw_struct = NULL);
};
#endif
	




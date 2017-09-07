#ifndef _LEDCONTROLLER_H_
#define _LEDCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

typedef struct LED
{	
	AHAL_LED_ID id;
	AHAL_LED_COLOR color; 
} LED_SETTING;

class LEDController : public HwController
{
	public:
		LEDController();
		virtual ~LEDController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct);
		virtual	int get_hw_info(void* hw_struct = NULL);
};
#endif
	




#ifndef _BUTTONCONTROLLER_H_
#define _BUTTONCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

typedef struct BUTTON
{	
	AHAL_BTN_ID id;
	AHAL_BTN_STATUS status;
} BUTTON_SETTING;

class ButtonController : public HwController
{
	public:
		ButtonController();
		virtual ~ButtonController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct = NULL);
		virtual	int get_hw_info(void* hw_struct);
		virtual int run_hw_info_detect();
};
#endif
	




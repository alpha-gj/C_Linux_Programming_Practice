#ifndef _BUTTONCONTROLLER_H_
#define _BUTTONCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

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
		virtual	int set_hw_info_by_struct(void* hw_struct);		/* Pass struct of data type for setting HW info */
		virtual	int get_hw_info_by_struct(void* hw_struct);		/* Return struct of date type for getting HW info */
		virtual int run_hw_info_detect();						/* TODO Need pthread?  Pass struct of data type for checking HW info */

		/* Button call function */
		//TODO select AHAL by paras
		
};

#endif
	




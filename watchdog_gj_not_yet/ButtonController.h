#ifndef _BUTTONCONTROLLER_H_
#define _BUTTONCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HWController.h"

typedef struct BUTTON
{	
	AHAL_BTN_ID id
	AHAL_BTN_STATUS status;
} BUTTON_SETTING;

	status = AHAL_BTN_STATUS_UNKNOWN;
	ret = AHAL_BTN_GetStatus(AHAL_BTN_ID_WPS, &status);
	if (ret != AHAL_RET_SUCCESS)
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_BTN_GetStatus fail\n");
    
	return (int)status;

class ButtonController : public HWController
{
	public:
		ButtonController();
		virtual ~ButtonController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info_by_struct(void hw_struct);		/* Pass struct of data type for setting HW info */
		virtual int run_hw_info_detect();							/* TODO Need pthread?  Pass struct of data type for checking HW info */
		virtual	int get_hw_info_by_struct(void hw_struct);		/* Return struct of date type for getting HW info */

		/* Button call function */
		//TODO select AHAL by paras
		
}
	




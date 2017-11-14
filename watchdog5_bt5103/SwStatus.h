#ifndef _SWSTATUS_H_
#define _SWSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "cam/cam.h"
#include "tinydb/tinydb.h"
#include "command.h"
#include "HwManager.h"

/* AHAL LIBS */
#include "AHAL_Type.h"
#include "AHAL_Cst.h"
#include "AHAL_Dbg.h"
#include "AHAL_Btn.h"
#include "AHAL_Ls.h"
#include "AHAL_PWM.h"
#include "AHAL_Icr.h"
#include "AHAL_Led.h"
#include "AHAL_Wifi.h"

class SwStatus
{
	protected:
		HwManager *hw_manager;

	public:
		SwStatus();
		virtual ~SwStatus();
		virtual int init() = 0;								/* To check or not according to DB value  */
		virtual int deinit() = 0;
		virtual int run_status_detect() = 0;				/* TODO Need pthread?  Pass struct of data type for checking HW info */
		virtual int pause_status_detect() = 0;
		virtual int continue_status_detect() = 0;
		virtual bool get_pause_detect_flag() = 0;
		virtual int set_status_info(void* status_struct) = 0;
		virtual int get_status_info(void* status_struct) = 0;
		virtual int update_thread_value() = 0;
};

#endif

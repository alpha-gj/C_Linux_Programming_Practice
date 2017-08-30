#ifndef _HWCONTROLLER_H_
#define _HWCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include "pib/pib.h"
#include "tinydb/tinydb.h"

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

/* HW define parameters */
#define TIME_INTERVAL		500000
#define DAY_TO_NIGHT_SEC	3 * (1000000/TIME_INTERVAL)
#define NIGHT_TO_DAY_SEC	7 * (1000000/TIME_INTERVAL)

/* FIXME Please put these define variables as below that should be in SPEC_DEFIN.h for project that be included in holder.h */
//#define RSSI_UB_DEF			(-73)
//#define RSSI_LB_DEF			(-85)

class HwController
{
	protected:
		HwController();
		virtual ~HwController();
/* TODO
		virtual bool RegisterHw() = 0;
		virtual bool UnrgisterHw() = 0;
 */
		/* For inheritance HW */
		virtual int init() = 0;
		virtual int deinit() = 0;
		virtual	int set_hw_info(void hw_struct) = 0;		/* Pass struct of data type for setting HW info */
		virtual int run_hw_info_detect() = 0;				/* TODO Need pthread?  Pass struct of data type for checking HW info */
		virtual	int get_hw_info(void hw_struct) = 0;		/* Return struct of date type for getting HW info */
}

#endif

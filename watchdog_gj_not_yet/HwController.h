#ifndef _HWCONTROLLER_H_
#define _HWCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
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
#include "AHAL_Audio.h"

class HwController
{
	protected:
		HwController();
/* TODO
		virtual bool RegisterHw() = 0;
		virtual bool UnrgisterHw() = 0;
 */
	public:
		virtual ~HwController();
		virtual int init() = 0;
		virtual int deinit() = 0;
		virtual	int set_hw_info(void *hw_struct) = 0;		/* Pass struct of data type for setting HW info */
		virtual	int get_hw_info(void *hw_struct) = 0;		/* Return struct of date type for getting HW info */
};

#endif

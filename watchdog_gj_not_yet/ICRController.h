#ifndef _ICRCONTROLLER_H_
#define _ICRCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

#define PWD_PERCENTAGE 100

typedef struct ICR
{	
	AHAL_CST_STATE state;
} ICR_SETTING;

class ICRController : public HwController
{
	public:
		ICRController();
		virtual ~ICRController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct);
		virtual	int get_hw_info(void* hw_struct = NULL);
};
#endif
	




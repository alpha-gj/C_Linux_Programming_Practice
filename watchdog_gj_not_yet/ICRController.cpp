#include <iostream>
#include "ICRController.h"

ICRController::ICRController()
{
	/* Do nothing */
}

ICRController::~ICRController()
{
	/* Do nothing */
}

int ICRController::init()
{
	fprintf(stderr, "ICR init\n");
	return (int)AHAL_ICR_Init();
}

int ICRController::deinit()
{
	fprintf(stderr, "ICR deinit\n");
	return (int)AHAL_ICR_UnInit();
}


int ICRController::set_hw_info(void *hw_struct) 
{
	int ret = AHAL_RET_FAIL;
	ICR_SETTING *icr_setting = (ICR_SETTING *) hw_struct;
	ret = AHAL_ICR_SetState(icr_setting->state);
	if (ret != AHAL_RET_SUCCESS) {
		AHAL_DBG_Print(AHAL_DBG_ERROR, "AHAL_ICR_SetState fail\n");
	}
	return (int)ret;
}

int ICRController::get_hw_info(void *)
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

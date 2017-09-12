#include <iostream>
#include "NetworkStatus.h"

NetworkStatus::NetworkStatus()
{
	/* Do Nothing */
}

NetworkStatus::~NetworkStatus()
{
	/* Do Nothing */
}

int NetworkStatus::init()
{
	fprintf(stderr, "NetworkStatus init\n");
	return (int)AHAL_RET_SUCCESS;
}

int NetworkStatus::deinit()
{
	fprintf(stderr, "NetworkStatus deinit\n");
	return (int)AHAL_RET_SUCCESS;
}

int NetworkStatus::run_status_detect()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int NetworkStatus::pause_status_detect() 
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int NetworkStatus::continue_status_detect()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

bool NetworkStatus::get_pause_detect_flag()
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int NetworkStatus::set_status_info(void *) 
{
	return(int)AHAL_RET_NOT_SUPPORT;
}

int NetworkStatus::get_status_info(void *) 
{ 
	return(int)AHAL_RET_NOT_SUPPORT;
}

#include "SwStatus.h"

SwStatus::SwStatus():hw_manager(HwManager::CreateHwManager())
{
	/* Do Nothing */
}

SwStatus::~SwStatus()
{
	HwManager::ReleaseHwManager();
}

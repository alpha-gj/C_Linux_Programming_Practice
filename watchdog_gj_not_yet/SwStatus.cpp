#include "SwStatus.h"

SwStatus::SwStatus()
{
	hw_manager = HwManager::CreateHwManager();
}

SwStatus::~SwStatus()
{
	HwManager::ReleaseHwManager();
}

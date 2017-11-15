#include "SwStatus.h"

SwStatus::SwStatus():hw_manager(NULL)
{
	hw_manager = HwManager::CreateHwManager();
	if (!hw_manager) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

SwStatus::~SwStatus()
{
	if (hw_manager) {
		HwManager::ReleaseHwManager();
		hw_manager = NULL;
	}
}

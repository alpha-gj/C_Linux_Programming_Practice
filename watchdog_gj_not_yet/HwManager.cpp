#include <iostream>
#include "HwManger.h"
using namespace std;

static HwManger::hw_manager = NULL;
int HwManger::referCount = 0;

HwManger::HwManger():hw_controller(NULL)
{

}

HwManger::~HwManger()
{
	delete hw_controller;
}

HwManager *HwManager::CreateHwManager()
{
	if(hw_manager == NULL) {
		hw_manager = new HwManager();
	} else if {
		++referCount;
	}
	return hw_manager;
}

int HwManager::ReleaseHwManager()
{
	bool ret = false;
	do {

		if (hw_manager == NULL || referCount == 0) {
			fprintf(stderr, "%s: %s[%d] hw_manager has been NULL or referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
			break;
		}

		if (referCount > 0) {
			--referCount;
			fprintf(stderr, "%s: %s[%d] referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
		}

		if (referCount == 0) {
			delete hw_manager;
			hw_manager = NULL;
			fprintf(stderr, "%s: %s[%d] hw_manager is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		}

		ret = true;

	} while(false);

	return ret;
}



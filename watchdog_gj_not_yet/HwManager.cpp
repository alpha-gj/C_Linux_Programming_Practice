#include "HwManager.h"
using namespace std;

HwManager *HwManager::hw_manager = NULL;
int HwManager::referCount = 0;

HwManager::HwManager()
{
	/* Do nothing */
}

HwManager::~HwManager()
{
	/* Do nothing */
}

HwManager *HwManager::CreateHwManager()
{
	if(hw_manager == NULL) {
		hw_manager = new HwManager();
	} else {
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


HwController *HwManager::ReturnHwControllerObjectByType(const char* hw_name)
{
	map<const char*, HwController *>::iterator in_map_hw_controller;
	HwController *hw_controller = NULL;

	in_map_hw_controller= map_hw_controller.find(hw_name);

	if (in_map_hw_controller == map_hw_controller.end()) {
		/* No result */
	} else {
		/* Get object address */
		hw_controller = in_map_hw_controller->second;
	}
	return hw_controller;
}

bool HwManager::init_hw_info_by_type(const char* hw_name)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);

	if (hw_controller == NULL)
		return false;
	else
		return hw_controller->init();
}

bool HwManager::deinit_hw_info_by_type(const char* hw_name)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);

	if (hw_controller == NULL)
		return false;
	else
		return hw_controller->deinit();
}

int HwManager::set_hw_info_by_type(const char* hw_name, void* hw_struct)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);
	if (hw_controller == NULL)
		return -1;
	else
		return hw_controller->set_hw_info(hw_struct);

}

int HwManager::get_hw_info_by_type(const char* hw_name, void* hw_struct)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);
	if (hw_controller == NULL)
		return -1;
	else
		return hw_controller->get_hw_info(hw_struct);
}

int HwManager::run_hw_info_detect_by_type(const char* hw_name)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);
	if (hw_controller == NULL)
		return -1;
	else
		return hw_controller->run_hw_info_detect();
}



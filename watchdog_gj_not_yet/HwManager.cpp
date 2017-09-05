#include "HwManager.h"
using namespace std;

HwManager *HwManager::hw_manager = NULL;
int HwManager::referCount = 0;

HwManager::HwManager()
{
	/*
	TODO Try to use Register and Unregister. 
	That you don't need add new code there, Just add xxxController.cpp & .h;
	*/
	/* New parts of HwController */
	map_hw_controller.insert(make_pair("BUTTON", new ButtonController()));
	map_hw_controller.insert(make_pair("WIFI", new WifiController()));
}

HwManager::~HwManager()
{
	/* Delete parts of HwController */
	map<string, HwController *>::iterator in_map_hw_controller;

	for (in_map_hw_controller = map_hw_controller.begin(); 
		 in_map_hw_controller != map_hw_controller.end(); 
		 in_map_hw_controller++) {
		delete in_map_hw_controller->second;
		in_map_hw_controller->second = NULL;
	}
}

int HwManager::init()
{
	map<string, HwController *>::iterator in_map_hw_controller;
	HwController *hw_controller = NULL;

	for (in_map_hw_controller = map_hw_controller.begin(); 
		 in_map_hw_controller != map_hw_controller.end(); 
		 in_map_hw_controller++) {
		hw_controller = in_map_hw_controller->second;
		hw_controller->init();
	}

	//TODO need to check
	return 0;
}

int HwManager::deinit()
{
	map<string, HwController *>::iterator in_map_hw_controller;
	HwController *hw_controller = NULL;

	for (in_map_hw_controller = map_hw_controller.begin(); 
		 in_map_hw_controller != map_hw_controller.end(); 
		 in_map_hw_controller++) {
		hw_controller = in_map_hw_controller->second;
		hw_controller->deinit();
	}

	//TODO need to check
	return 0;
}

HwManager *HwManager::CreateHwManager()
{
	if (hw_manager == NULL) {
		hw_manager = new HwManager();	//TODO Need to check successful or not
		hw_manager->init();				//TODO Need to check successful or not
	} 

	if (hw_manager) {
		++referCount;
		printf("HwManager refer Count is %d\n", referCount);
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
			hw_manager->deinit(); //TODO Need to check successful or not
			delete hw_manager;
			hw_manager = NULL;
			fprintf(stderr, "%s: %s[%d] hw_manager is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		}

		ret = true;

	} while(false);

	return ret;
}


HwController *HwManager::ReturnHwControllerObjectByType(const char* hw_controller_name)
{
	map<string, HwController *>::iterator in_map_hw_controller;
	HwController *hw_controller = NULL;
	string s_hw_controller_name(hw_controller_name);

	in_map_hw_controller= map_hw_controller.find(s_hw_controller_name);

	if (in_map_hw_controller != map_hw_controller.end()) {
		/* Get object address */
		hw_controller = in_map_hw_controller->second;
	} else {
		/* No result */
	}
	return hw_controller;
}

int HwManager::init_hw_info_by_type(const char* hw_name)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);

	if (hw_controller == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return hw_controller->init();
}

int HwManager::deinit_hw_info_by_type(const char* hw_name)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);

	if (hw_controller == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return hw_controller->deinit();
}

int HwManager::set_hw_info_by_type(const char* hw_name, void* hw_struct)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);
	if (hw_controller == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return hw_controller->set_hw_info(hw_struct);

}

int HwManager::get_hw_info_by_type(const char* hw_name, void* hw_struct)
{
	HwController *hw_controller = ReturnHwControllerObjectByType(hw_name);
	if (hw_controller == NULL) {
		return -1; //TODO need to verify, not find out or function is not supported
	} else {
		return hw_controller->get_hw_info(hw_struct);
	}
}


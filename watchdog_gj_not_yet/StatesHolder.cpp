#include "StatesHolder.h"
#include <iostream>
using namespace std;

StatesHolder *StatesHolder::holder = NULL;
int StatesHolder::referCount = 0;


StatesHolder::StatesHolder():mainStates(POWERUP),oldMainStates(POWERUP)
{
	/* Insert something that you want to detect, such as TWT, button, network status, lightsensor(day,night mode) */
	map_sw_status.insert(make_pair("LEDStatus", new LEDStatus()));
	map_sw_status.insert(make_pair("NetworkStatus", new NetworkStatus()));
	map_sw_status.insert(make_pair("ButtonStatus", new ButtonStatus()));
	map_sw_status.insert(make_pair("LightSensorStatus", new LightSensorStatus()));
}

StatesHolder::~StatesHolder()
{
	map<string, SwStatus *>::iterator in_map_sw_status;

	for (in_map_sw_status = map_sw_status.begin(); 
		 in_map_sw_status != map_sw_status.end();
		 in_map_sw_status++) {
		delete in_map_sw_status->second;
		in_map_sw_status->second = NULL;
	}
}

int StatesHolder::init()
{
	map<string, SwStatus *>::iterator in_map_sw_status;
	SwStatus *sw_status = NULL;

	for (in_map_sw_status = map_sw_status.begin(); 
		 in_map_sw_status != map_sw_status.end();
		 in_map_sw_status++) {
		sw_status = in_map_sw_status->second;
		sw_status->init();
	}

	//TODO need to check
	return 0;
}

int StatesHolder::deinit()
{
	map<string, SwStatus *>::iterator in_map_sw_status;
	SwStatus *sw_status = NULL;

	for (in_map_sw_status = map_sw_status.begin(); 
		 in_map_sw_status != map_sw_status.end();
		 in_map_sw_status++) {
		sw_status = in_map_sw_status->second;
		sw_status->deinit();
	}

	//TODO need to check
	return 0;
}

StatesHolder *StatesHolder::CreateStatesHolder()
{
	if (holder == NULL)  {
		holder = new StatesHolder();
		holder->init();
	}
	if (holder) {
		++referCount;
		printf("StatesHolder refer Count is %d\n", referCount);
	}
	return holder;
}

int StatesHolder::ReleaseStatesHolder()
{
	bool ret = false;
	do {

		if (holder == NULL || referCount == 0) {
			fprintf(stderr, "%s: %s[%d] holder has been NULL or referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
			break;
		}

		if (referCount > 0) {
			--referCount;
			fprintf(stderr, "%s: %s[%d] referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
		}

		if (referCount == 0) {
			holder->deinit();
			delete holder;
			holder = NULL;
			fprintf(stderr, "%s: %s[%d] holder is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		}

		ret = true;

	} while(false);

	return ret;
}

SwStatus *StatesHolder::ReturnSwStatusObjectByType(const char* sw_status_name)
{
	map<string, SwStatus *>::iterator in_map_sw_status;
	SwStatus *sw_status = NULL;
	string s_sw_status_name(sw_status_name);

	in_map_sw_status = map_sw_status.find(s_sw_status_name);

	if (in_map_sw_status != map_sw_status.end()) {
		/* Get object address */
		sw_status = in_map_sw_status->second;
	} else {
		/* No result */
		fprintf(stderr, "%s: %s[%d] We can't find the object in MAP'\n", __FILE__, __FUNCTION__, __LINE__);
	}
	return sw_status;
}

int StatesHolder::init_status_detect_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->init();
}

int StatesHolder::deinit_status_detect_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->deinit();
}

int StatesHolder::run_status_detect_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->run_status_detect();
}

int StatesHolder::pause_status_detect_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->pause_status_detect();
}

int StatesHolder::continue_status_detect_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->continue_status_detect();
}

int StatesHolder::get_pause_detect_flag_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->get_pause_detect_flag();
}

int StatesHolder::set_status_info_by_type(const char* status_name, void* status_struct)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);
	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->set_status_info(status_struct);
}

int StatesHolder::get_status_info_by_type(const char* status_name, void* status_struct)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);
	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->get_status_info(status_struct);
}

int StatesHolder::update_thread_value_by_type(const char* status_name)
{
	SwStatus *sw_status = ReturnSwStatusObjectByType(status_name);

	if (sw_status == NULL)
		return -1; //TODO need to verify, not find out or function is not supported
	else
		return sw_status->update_thread_value();
}

void StatesHolder::SetMainStates(MAINSTATES s)
{
	oldMainStates = mainStates;
	mainStates = s;
}

MAINSTATES StatesHolder::GetMainStates()
{
	MAINSTATES t = mainStates;
	return t;
}

MAINSTATES StatesHolder::GetOldMainStates()
{
	MAINSTATES t = oldMainStates;
	return t;
}


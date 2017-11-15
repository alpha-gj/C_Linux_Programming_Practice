#include "StatesHandler.h"
using namespace std;

StatesHandler *StatesHandler::handler = NULL;
int StatesHandler::referCount = 0;

StatesHandler::StatesHandler():holder(NULL)
{
	holder = StatesHolder::CreateStatesHolder();
	if (!holder) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

StatesHandler::~StatesHandler()
{
	if (holder) {
		StatesHolder::ReleaseStatesHolder();
		holder = NULL;
	}
}

int StatesHandler::init()
{
	return 0;
}

int StatesHandler::deinit()
{
	return 0;
}

StatesHandler *StatesHandler::CreateStatesHandler()
{
	if (handler == NULL)  {
		handler = new StatesHandler();
	}

	if (handler) {
		++referCount;
		printf("StatesHandler refer Count is %d\n", referCount);
	}
	return handler;
}

int StatesHandler::ReleaseStatesHandler()
{
	bool ret = false;
	do {

		if (handler == NULL || referCount == 0) {
			fprintf(stderr, "%s: %s[%d] handler has been NULL or referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
			break;
		}

		if (referCount > 0) {
			--referCount;
			fprintf(stderr, "%s: %s[%d] referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
		}

		if (referCount == 0) {
			delete handler;
			handler = NULL;
			fprintf(stderr, "%s: %s[%d] handler is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		}

		ret = true;

	} while(false);

	return ret;
}

int StatesHandler::run_parsing_command_by_main_states()
{
	//FIXME check resource
	IPCHandler *sw_states = NULL;
	while (!get_quit() && !get_reload()) {
		MAINSTATES states  = holder->GetMainStates();
		sw_states = ReturnMainIPCHandlerObject(states);
		sw_states->init();
		
		/* Main loop */
		sw_states->run_parsing_command();

		sw_states->deinit();
		delete sw_states;
		sw_states = NULL;
	}
	return 0;
}

int StatesHandler::do_event_by_main_states(EVENT sw_event_name)
{
	//FIXME it seems like that it can do better to check sw_states
	int ret = -1;
	IPCHandler *sw_states = NULL;

	do { 
		MAINSTATES states  = holder->GetMainStates();
		sw_states = ReturnMainIPCHandlerObject(states);
		
		if (sw_states == NULL) {
			INFO("ReturnMainIPCHandlerObject fail, states is %d", states);
			break;
		}
		
		printf("%s %s\n", __FILE__,__FUNCTION__);
		sw_states->DoEventByType(sw_event_name); //FIXME Need to tell it exsited or not

	} while(false);

	if (sw_states)
	{
		delete sw_states;
		sw_states = NULL;
	}
	
	return ret;
}

IPCHandler *StatesHandler::ReturnMainIPCHandlerObject(MAINSTATES states)
{
	switch (states) {
		case BOOTING:
			INFO("enter BOOTING");
			return new BootingHandler();
			break;
		case FACTORY_RESET:
			INFO("enter FACTORY_RESET");
			return new FactoryResetHandler();
			break;
		default:
			INFO("BUG@%d", __LINE__);
			break;
	}
	return NULL;
}

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
	} else {
		fprintf(stderr, "Could't Create the StatesHandler again!\n");
		return NULL;
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
	IPCHandler *sw_states = NULL;
	//FIXME check resource
	while (!get_quit() && !get_reload()) {
		MAINSTATES states  = holder->GetMainStates();
		sw_states = ReturnMainIPCHandlerObject(states);
		sw_states->init();
		sw_states->run_parsing_command();
		sw_states->deinit();
		delete sw_states;
	}
	return 0;
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
			return new BootingHandler();
			break;
	}
	return NULL;
}

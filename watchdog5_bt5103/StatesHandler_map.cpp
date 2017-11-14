#include "StatesHandler.h"
using namespace std;

StatesHandler *StatesHandler::handler = NULL;
int StatesHandler::referCount = 0;

StatesHandler::StatesHandler()
{
	/* Insert something that you handler */
	map_sw_states.insert(make_pair(BOOTING, new BootingHandler()));
	map_sw_states.insert(make_pair(FACTORY_RESET, new FactoryResetHandler()));
}

StatesHandler::~StatesHandler()
{
	map<MAINSTATES, IPCHandler *>::iterator in_map_sw_states;

	for (in_map_sw_states = map_sw_states.begin(); 
		 in_map_sw_states != map_sw_states.end();
		 in_map_sw_states++) {
		delete in_map_sw_states->second;
		in_map_sw_states->second = NULL;
	}
}

int StatesHandler::init()
{
	MAINSTATES states = StatesHolder::GetMainStates();
	sw_states = ReturnMainIPCHandlerObject(states);

	return sw_states->init();
}

int StatesHandler::deinit()
{
	MAINSTATES old_states = StatesHolder::GetOldMainStates();
	sw_states = ReturnMainIPCHandlerObject(old_states);

	return sw_states->deinit();
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
	MAINSTATES states = StatesHolder::GetMainStates();
#if 0
	sw_states = (IPCHandler*) ReturnMainIPCHandlerObject(states);
	//Althrough it seems like that do same thing as new BootingHandler(), its IPC can't work that the object got from ReturnMainIPCHandlerObject(states);
#else
	sw_states = new BootingHandler();
#endif
	sw_states->init();
	sw_states->run_parsing_command();
	return 0;
}

IPCHandler *StatesHandler::ReturnMainIPCHandlerObject(MAINSTATES states)
{
	map<MAINSTATES, IPCHandler *>::iterator in_map_sw_states;
	IPCHandler *ipchandler = NULL;
	in_map_sw_states = map_sw_states.find(states);

	if (in_map_sw_states != map_sw_states.end()) {
		/* Get object address */
		ipchandler = in_map_sw_states->second;
	} else {
		/* No result */
		fprintf(stderr, "%s: %s[%d] We can't find the object in MAP'\n", __FILE__, __FUNCTION__, __LINE__);
	}
	return ipchandler;
}

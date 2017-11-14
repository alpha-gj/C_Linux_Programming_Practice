#include "FactoryResetHandler.h"

FactoryResetHandler::FactoryResetHandler()
{
	fprintf(stderr, "New FACTORY_RESET\n");
	/* Do nothing */
}

FactoryResetHandler::~FactoryResetHandler()
{
	fprintf(stderr, "Delete FACTORY_RESET\n");
	/* Do nothing */
}

int FactoryResetHandler::init()
{
	/* TODO Let's get it smiple */
	fprintf(stderr, "init FACTORY_RESET\n");
	return 0;
}

int FactoryResetHandler::deinit()
{
	fprintf(stderr, "deinit FACTORY_RESET\n");
	return 0;
}

int FactoryResetHandler::handle_ipc_depend_on_status(IpcCommand cmd)
{
	int ret = 0;
	switch(cmd.id) {
		default:
			ret = -1;
			break;
	}
	return ret;
}

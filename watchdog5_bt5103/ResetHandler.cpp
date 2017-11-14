#include "ResetHandler.h"

ResetHandler::ResetHandler()
{

}

ResetHandler::~ResetHandler()
{

}

int ResetHandler::init()
{
	if (IPCHandler::init()) {
		return -1;
	} else {
		Color red(LED_RED, BLINK, FIXED_VALUE_FOR_INTERACTIVE, BLINK_PERIOD);
		Color green(LED_GREEN, BLINK, FIXED_VALUE_FOR_INTERACTIVE, BLINK_PERIOD);
		led.setBlink(red, green);
		system("factoryReset > /dev/null 2>&1");
		system("reboot > /dev/null 2>&1");
		return 0;
	}
}

int ResetHandler::release()
{
	IPCHandler::release();
	return 0;
}

int ResetHandler::handle_ipc_depend_on_status(IpcCommand /*cmd*/)
{
	return -1;
}

MAINSTATES ResetHandler::GetMainHandlerState()
{
	return RESET;
}

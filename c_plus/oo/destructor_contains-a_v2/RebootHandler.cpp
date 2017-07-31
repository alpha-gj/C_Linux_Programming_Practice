#include "RebootHandler.h"

RebootHandler::RebootHandler()
{

}

RebootHandler::~RebootHandler()
{

}

int RebootHandler::init()
{
	if (IPCHandler::init()) {
		return -1;
	} else {
		/*
		Color color(LED_YELLOW, BLINK, FIXED_VALUE_FOR_INTERACTIVE, BLINK_PERIOD);
		led.setColor(color);
		*/
		system("reboot > /dev/null 2>&1");
		return 0;
	}
}

int RebootHandler::release()
{
	IPCHandler::release();
	return 0;
}

int RebootHandler::handle_ipc_depend_on_status(IpcCommand /*cmd*/)
{
	return -1;
}

MAINSTATES RebootHandler::GetMainHandlerState()
{
	return REBOOT;
}

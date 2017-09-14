#include "PowerUpHandler.h"

PowerUpHandler::PowerUpHandler()
{
	/* Do nothing */
}

PowerUpHandler::~PowerUpHandler()
{
	/* Do nothing */
}

int PowerUpHandler::init()
{
	if (IPCHandler::init()) {
		return -1;
	} else {
		//Color white(LED_WHITE, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
		//led.setColor(white);
		//check_ethernet
		//check_associate();
		//if has ip -> SetMainStates(CONNECTED);
		//if no steup -> SetMainStates(SETUP);
		//if has setup, no connect -> SetMainStates(DISCONNECTED);
		return 0;
	}
}

int PowerUpHandler::release()
{
	IPCHandler::release();
	return 0;
}

int PowerUpHandler::handle_ipc_depend_on_status(IpcCommand cmd)
{
	int ret = 0;
	switch(cmd.id) {
		default:
			ret = -1;
			break;
	}
	return ret;
}

MAINSTATES PowerUpHandler::GetMainHandlerState()
{
	return POWERUP;
}

void PowerUpHandler::check_associate()
{
	/*
	if (probe_link()) {
		holder->SetAssociated(true);
	} else {
		IF_INFO if_info;
		if (does_wifi_associated((char *)PLATFORM_DEV_WLAN) &&
				get_active_interface(PLATFORM_DEV_BR, &if_info) &&
				if_info.ipaddr.s_addr != 0) {
			holder->SetAssociated(true);
		} else {
			holder->SetAssociated(false);
		}
	}
	*/
}
/*
POWER_LED_STATE IPCHandler::set_pled_state_by_link_state() 
{
	POWER_LED_STATE pled_state = PLED_BOOTING;
	probe_link()
	if(get_stream_count() > 0) {
		set_pled_state(PLED_ACTIVE);
	} else if(check_associate()) {
		set_pled_state(PLED_CLIENT_MODE);
	} else if(system("pidof bluetoothd") == 0) {
		set_pled_state(PLED_BT_MODE);
	} else {
		#ifndef WIFI_STA
		set_pled_state(PLED_AP_MODE);
		#else
		set_pled_state(PLED_DISASSOCIATED);
		#endif
	}
	return ;
}
*/
/* FIXME for this to setMainStates */

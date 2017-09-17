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
		/* TODO Let's get it smiple */
		printf("hello power up\n");
		ICR_SETTING icr_setting {
			.state = AHAL_CST_STATE_ON
		};
		hw_manager->set_hw_info_by_type("ICR", &icr_setting);
		check_associate(); /* For PowerUp state */
		return 0;
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
	LED_STATUS_SETTING led_status_setting;
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);

	IF_INFO if_info;
	if (does_wifi_associated(NULL) && get_active_interface(PLATFORM_DEV_BR, &if_info) && if_info.ipaddr.s_addr != 0) {
		if (led_status_setting.pled_state != PLED_CLIENT_MODE  && (led_status_setting.pled_state = PLED_CLIENT_MODE))
			holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	} else {
		if (led_status_setting.pled_state != PLED_BT_MODE  && (led_status_setting.pled_state = PLED_BT_MODE))
			holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}	
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

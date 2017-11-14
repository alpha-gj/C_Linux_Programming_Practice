#include "BootingHandler.h"

BootingHandler::BootingHandler()
{
	fprintf(stderr, "New BOOTING\n");
	/* Do nothing */
}

BootingHandler::~BootingHandler()
{
	fprintf(stderr, "Delete BOOTING\n");
	/* Do nothing */
}

int BootingHandler::init()
{
	/* TODO Let's get it smiple */
	fprintf(stderr, "init BOOTING\n");
	ICR_SETTING icr_setting {
		.state = AHAL_CST_STATE_ON
	};
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);
	check_associate(); /* For PowerUp state */
	return 0;
}

int BootingHandler::deinit()
{
	fprintf(stderr, "deinit BOOTING\n");
	return 0;
}

int BootingHandler::handle_ipc_depend_on_status(IpcCommand cmd)
{
	int ret = 0;
	switch(cmd.id) {
		case 100:
			printf("test 100\n");
			holder->SetMainStates(FACTORY_RESET);
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}

void BootingHandler::check_associate()
{
	//FIXME It should check network link up or not 
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

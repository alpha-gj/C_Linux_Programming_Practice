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
	/* check_associate(); /1* For PowerUp state *1/ */
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
		default:
			ret = -1;
			break;
	}

	return ret;
}

int BootingHandler::DoEventByType(EVENT sw_event_name)
{
	int ret = 0;

	switch(sw_event_name) {
		case BUTTON_EVENT:
			DoButtonEvent();
			break;
		default:
			ret = -1;
			break;
	}

	return ret;
}

int BootingHandler::DoButtonEvent()
{
	HwManager *hw_manager = HwManager::CreateHwManager();		/* Get Hw info */
	StatesHolder *holder = StatesHolder::CreateStatesHolder();	/* Keep Button State for Reset Button */

	BUTTON_STATE button_state;
	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};
	unsigned int press_count = 0;
	unsigned int release_timeout_count = 0;

	while (!get_quit() && !get_reload() && !IsStatesChanged()) {
		/* Get Button Status */
		hw_manager->get_hw_info_by_type("BUTTON", &button_setting);
		
		//TODO
		/* Need  to press reset button in 3 secs and check network. Should it use timeout to enter network disconnect? */
		if (release_timeout_count >= RELEASE_RESET_BUTTON_TIMEOUT_INTERVAL_SEC)
		{
			fprintf(stderr, "Timeout\n");
		}
		
		if (button_setting.status == AHAL_BTN_STATUS_PRESSED) {
			printf("count is %d\n", press_count);
			press_count++;
		} else if (button_setting.status == AHAL_BTN_STATUS_RELEASE) {
			press_count = 0;
			release_timeout_count++;
		} else if (button_setting.status == AHAL_BTN_STATUS_UNKNOWN) {
			ERROR("RESET BUTTON Get AHAL_BTN_STATUS_UNKNOWN");
			press_count = 0;
		}
		else {
			/* Do Nothing */
		}
	
		/* Get Buton Status */
		holder->get_status_info_by_type("ButtonStatus", &button_state);

		/* Count and return button state */
		if(button_state.reset_button_state == RESET_READY) {
			button_state.reset_button_state = return_button_state_by_press_count(press_count);
		}

		switch (button_state.reset_button_state) {
			case RESET_READY:
				break;
			case RESET_STANDBY:
				INFO("Set Reset LED");
				fprintf(stderr, "Set Reset LED!\n");
				SetMainStates(FACTORY_RESET);
				break;
			default:
				INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
				break;
		}

		/* Set Buton Status */
		holder->set_status_info_by_type("ButtonStatus", &button_state);

		sleep(1);
	};

	StatesHolder::ReleaseStatesHolder();
	HwManager::ReleaseHwManager();
	return 0;
}

RESET_BUTTON_STATE BootingHandler::return_button_state_by_press_count(int press_count)
{
	if (press_count >= PRESS_RESET_BUTTON_INTERVAL_SEC) {
		return RESET_STANDBY;
	}
	else {
		return RESET_READY;
	}
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

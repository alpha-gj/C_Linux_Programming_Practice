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

int FactoryResetHandler::DoEventByType(EVENT sw_event_name)
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

int FactoryResetHandler::DoButtonEvent()
{
	HwManager *hw_manager = HwManager::CreateHwManager();		/* Get Hw info */
	StatesHolder *holder = StatesHolder::CreateStatesHolder();	/* Keep Button State for Reset Button */
	BUTTON_STATE button_state;
	BUTTON_SETTING button_setting {
		.id = AHAL_BTN_ID_RESET,
		.status = AHAL_BTN_STATUS_UNKNOWN
	};

	while (!get_quit() && !get_reload() && !IsStatesChanged()) {

		/* Get Button Status */
		hw_manager->get_hw_info_by_type("BUTTON", &button_setting);

		/* Get Button States */
		holder->get_status_info_by_type("ButtonStatus", &button_state);

		switch (button_state.reset_button_state) {
			case RESET_STANDBY:
				send_ipccmd(WATCHDOG_SOCKET_NAME, CMD_DET_FACTORY_BUTTON);
				button_state.reset_button_state = RESET_LAUNCH;
				break;
			case RESET_LAUNCH:
				if (button_setting.status == AHAL_BTN_STATUS_RELEASE) {
					INFO("Do RESET");
					fprintf(stderr, "Do Reset!\n");
					send_ipccmd(WATCHDOG_SOCKET_NAME, CMD_FACTORY_RESET_ACT);
					button_state.reset_button_state = RESET_READY;
				} else if (button_setting.status == AHAL_BTN_STATUS_PRESSED) {
					fprintf(stderr, "You can Release button to do Reset right now!\n");
				}
				break;
			default:
				INFO("BUG@%s %d", __FUNCTION__,  __LINE__);
				break;
		}
		
		/* Set Buton Status */
		holder->set_status_info_by_type("ButtonStatus", &button_state);
		sleep(1);
	}

	StatesHolder::ReleaseStatesHolder();
	HwManager::ReleaseHwManager();
}

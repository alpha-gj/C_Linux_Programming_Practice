#include "PowerUpHandler.h"

PowerUpHandler::PowerUpHandler()
{
}

PowerUpHandler::~PowerUpHandler()
{
}

int PowerUpHandler::init()
{
	if (IPCHandler::init()) {
		return -1;
	} else {
		check_associate();
		Color white(LED_WHITE, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
		led.setColor(white);
		ack = holder->GetWpsStates();
		keepButtonStates = holder->GetButtonStates();
		if (keepButtonStates.buttonOn) {
			holder->SetMainStates(POWERUPWITHBUTTON);
		} else {
			enterNormalBootup();
		}
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
		case CMD_WPS_START_BY_CGI:
			if (noWifiSetting) {
				pthread_t p = 0;
				start_thread((void*)runWPSStartThread, p, holder);
			} else {
				ret = -1;
			}
			break;
		case CMD_SETTING_BY_CGI: {
									 Color ready(LED_GREEN, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
									 led.setColor(ready);
									 lib_play_audio_with_volume(WPS_BEGIN_TONE);
									 holder->SetMainStates(SETUP);
									 holder->SetWpsStates(WPS_OFF);
								 }
								 break;
		default:
								 ret = -1;
								 break;
	}
	return ret;
}

int PowerUpHandler::handle_select_time_out()
{
	if (waitHeartbeatNotifyCount >= 0) {
		--waitHeartbeatNotifyCount;
	}
	if (waitHeartbeatNotifyCount < 0) {
		holder->SetMainStates(TROUBLE_SHOOTING);
	}
	button_states curButtonStates = holder->GetButtonStates();
	if (curButtonStates.buttonOn != keepButtonStates.buttonOn) {
		if ((curButtonStates.buttonOn == true) &&
				(keepButtonStates.buttonOn == false)) {
			// start to press
		} else {
			// stop to press
			switch (ack) {
				case WPS_PAIRING:
					holder->SetMainStates(SETUP);
					holder->SetWpsStates(WPS_PAIRING);
					break;
				default:
					// ack as normal
					// do nothing
					break;
			}
		}
	} else {
		if (curButtonStates.buttonOn == true) {
			if ((ack == WPS_OFF) &&
					(diff_curr_uptime_count(curButtonStates.buttonOnTime) > ENTER_WPS_COUNT)) {
				Color ready(LED_GREEN, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
				led.setColor(ready);
				lib_play_audio_with_volume(WPS_BEGIN_TONE);
				ack = WPS_PAIRING;
			}
		}
	}
	keepButtonStates = curButtonStates;
	return 0;
}

MAINSTATES PowerUpHandler::GetMainHandlerState()
{
	return POWERUP;
}

int PowerUpHandler::handle_network_states()
{
	if (holder->GetNeedHandleACK()) {
		if (holder->IsPanelACK()) {
			holder->SetMainStatesByCameraMode();
		} else {
			holder->SetMainStates(TROUBLE_SHOOTING);
		}
		holder->CleanNeedHandleACK();
		holder->CleanNeedHandleAssociated();
	}
	if (holder->GetNeedHandleAssociated()) {
		if (holder->IsAssociated()) {
			holder->SetMainStatesByCameraMode();
			holder->SetUnderLoseConnectionNeedWait(false);
		} else {
			// no change
		}
		holder->CleanNeedHandleAssociated();
		return 0;
	}
	return 0;
}

int PowerUpHandler::enterNormalBootup()
{
	int ret = 0;
	try	{
		TinyDB db;
		db.init("Wireless");
		string v = cat("/tmp/ethernet");
		if (!v.compare("0")) {
			if (!db.getByte("Enable")) {
				//lib_play_audio_with_volume(PLAY_INITIALING_START_VOICE);
				Color green(LED_GREEN, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
				led.setColor(green);
				lib_play_audio_with_volume(PLAY_BOOTINGUP_TONE);
				lib_play_audio_with_volume(PLAY_BOOTINGUP_VOICE);
				Color ready(LED_GREEN, BREATHING, FIXED_VALUE_FOR_INTERACTIVE, BREATHING_PERIOD);
				led.setColor(ready);
				waitHeartbeatNotifyCount = 0x7fffffff;
				noWifiSetting = true;
			} else {
				Color color3(LED_WHITE, BREATHING, FIXED_VALUE_FOR_INTERACTIVE, BREATHING_PERIOD);
				led.setColor(color3);
				waitHeartbeatNotifyCount = WAIT_HEART_BEAT_COUNT;
				noWifiSetting = false;
			}
		} else {
			Color color3(LED_WHITE, BREATHING, FIXED_VALUE_FOR_INTERACTIVE, BREATHING_PERIOD);
			led.setColor(color3);
			waitHeartbeatNotifyCount = WAIT_HEART_BEAT_COUNT;
			noWifiSetting = false;
		}
		db.release();
	} catch(TinyDBError & error) {
		ERROR("%s", error.what());
		return -1;
	}
	return ret;
}
void PowerUpHandler::check_associate()
{
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

}

void * PowerUpHandler::runWPSStartThread(void *args)
{
	StatesHolder *h = (StatesHolder *)args;
	sleep(1);
	h->SetMainStates(SETUP);
	h->SetWpsStates(WPS_PAIRING);
	pthread_exit(NULL);
}

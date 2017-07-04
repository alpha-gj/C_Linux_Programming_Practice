#include "TwtHandler.h"

TwtHandler::TwtHandler()
{
	sender.Init(VIVINT_CHANNEL);
}

TwtHandler::~TwtHandler()
{

}

int TwtHandler::init()
{
	if (IPCHandler::init()) {
		return -1;
	} else {
		system(AEC_RESET);
		TWTSTATES s = holder->GetTwtStates();
		if (s == CALL_FROM_OUTSIDE) {
			Color outside(LED_BLUE, SOLID, ALL_VALUE_FOR_TWT, SOLID_PERIOD);
			led.setColor(outside);
			lib_play_audio_with_volume(PLAY_TWT_BEGIN_TONE);
			holder->SetTwtStates(TALKING);
			timeout_count = 0;
		} else if (s == CALLING_OUT) {
			start_thread_join((void *)&run_calling_out_thread, pid, &needStopCallingOut);
			needStopCallingOut = false;
			Color extend(LED_BLUE, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
			led.setColor(extend);
			usleep(1020000);
			Color callout(LED_BLUE, BREATHING, FIXED_VALUE_FOR_INTERACTIVE, BREATHING_PERIOD);
			led.setColor(callout);
			holder->SetTwtStates(CALLING_OUT);
			timeout_count = WAIT_CALL_OUT_COUNT;
		} else {
			holder->SetMainStates(NORMAL);
			holder->SetTwtStates(CALL_OFF);
		}
		return 0;
	}
}

int TwtHandler::release()
{
	IPCHandler::release();
	return 0;
}

int TwtHandler::handle_ipc_depend_on_status(IpcCommand cmd)
{
	int ret = 0;
	switch (cmd.id) {
		case CMD_TWT_END:
			changeColor();
			enterNormal();
			break;
		case CMD_TWT_BEGIN:
			{
				needStopCallingOut = true;
				if (pid > 0) {
					pthread_join(pid, NULL);
					pid = 0;
				}
				Color outside(LED_BLUE, SOLID, ALL_VALUE_FOR_TWT, SOLID_PERIOD);
				led.setColor(outside);
				lib_play_audio_with_volume(PLAY_TWT_BEGIN_TONE);
				holder->SetTwtStates(TALKING);
				timeout_count = 0;
			} break;
		default:
			ret = -1;
			break;
	}
	return ret;
}

MAINSTATES TwtHandler::GetMainHandlerState()
{
	return TWT;
}

int TwtHandler::handle_select_time_out()
{
	if ((holder->GetTwtStates() == CALLING_OUT) &&
			(timeout_count > 0)) {
		--timeout_count;
		if (timeout_count <= 0) {
			changeColor();
			enterNormal();
		}
	}
	button_states curButtonStates = holder->GetButtonStates();
	if (curButtonStates.buttonOn != keepButtonStates.buttonOn) {
		if ((curButtonStates.buttonOn == true) &&
				(keepButtonStates.buttonOn == false)) {
			// start to press
			changeColor();
		} else {
			// stop to press
			enterNormal();
		}
	}
	keepButtonStates = curButtonStates;
	return 0;
}

void TwtHandler::changeColor()
{
	if (holder->GetTwtStates() == CALLING_OUT) {
		needStopCallingOut = true;
		if (pid > 0) {
			pthread_join(pid, NULL);
			pid = 0;
		}
	}
	Color outside(LED_WHITE, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
	led.setColor(outside);
	lib_play_audio_with_volume(PLAY_TWT_END_TONE);
}

void TwtHandler::enterNormal()
{
	memset(&pack, 0x00, sizeof(pack));
	pack.iEventMask = TWO_WAY_TALK_EVENT;
	pack.iSubEventMask = TWT_END_EVENT;
	sender.SendPackage(&pack);
	holder->SetMainStates(NORMAL);
	holder->SetTwtStates(CALL_OFF);
}

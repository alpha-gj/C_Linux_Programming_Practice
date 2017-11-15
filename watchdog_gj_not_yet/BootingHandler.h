#ifndef _BOOTINGHANDLER_H_
#define _BOOTINGHANDLER_H_
#include "IPCHandler.h"
#include "StatesHolder.h"
#include "HwManager.h"

#define PRESS_RESET_BUTTON_INTERVAL_SEC 3
#define RELEASE_RESET_BUTTON_TIMEOUT_INTERVAL_SEC 3

class BootingHandler : public IPCHandler
{
	private:
		int DoButtonEvent();
		static RESET_BUTTON_STATE return_button_state_by_press_count(int press_count);
		void check_associate();
	public:
		BootingHandler ();
		virtual ~BootingHandler ();
		virtual int init();
		virtual int deinit();
		virtual int handle_ipc_depend_on_status(IpcCommand cmd);
		virtual int DoEventByType(EVENT sw_event_name);
};

#endif

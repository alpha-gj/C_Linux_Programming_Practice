#ifndef _FACTORYRESETHANDLER_H_
#define _FACTORYRESETHANDLER_H_
#include "IPCHandler.h"

class FactoryResetHandler : public IPCHandler
{
	private:
		int DoButtonEvent();
		static RESET_BUTTON_STATE return_button_state_by_press_count(int press_count);
	public:
		FactoryResetHandler ();
		virtual ~FactoryResetHandler ();
		virtual int init();
		virtual int deinit();
		virtual int handle_ipc_depend_on_status(IpcCommand cmd);
		virtual int DoEventByType(EVENT sw_event_name);
};

#endif

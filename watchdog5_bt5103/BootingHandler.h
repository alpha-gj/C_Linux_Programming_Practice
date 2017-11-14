#ifndef _BootingHandler_H_
#define _BootingHandler_H_
#include "IPCHandler.h"

#define PRESS_RESET_BUTTON_INTERVAL_SEC 3

class BootingHandler : public IPCHandler
{
	private:
		void check_associate();

	public:
		BootingHandler ();
		virtual ~BootingHandler ();
		virtual int init();
		virtual int deinit();
		virtual int handle_ipc_depend_on_status(IpcCommand cmd);

};

#endif

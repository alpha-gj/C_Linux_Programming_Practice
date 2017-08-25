#ifndef _IPCHANDLER_H_
#define _IPCHANDLER_H_
#include "StatesHolder.h"

class IPCHandler
{
public:
	IPCHandler ();
	virtual ~IPCHandler ();
	virtual int init() = 0;
	virtual int release() = 0;
	virtual int handle_select_time_out();
	virtual int handle_network_states();
	virtual int run_parsing_command();
	virtual bool GoNextState();
	virtual MAINSTATES GetMainHandlerState() = 0;

protected:
	virtual int handle_factory_reset();

protected:
	StatesHolder *holder;
};

#endif

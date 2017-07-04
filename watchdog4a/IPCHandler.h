#ifndef _IPCHANDLER_H_
#define _IPCHANDLER_H_
#include "ipc/ipc_cmd.h"
#include "MulCast/MulCast.h"
#include "command.h"
#include "StatesHolder.h"
#include "tinydb/tinydb.h"
#include "Common.h"
#include "todo.h"
#include "Variable.h"

class IPCHandler
{
public:
	IPCHandler ();
	virtual ~IPCHandler ();
	virtual int init() = 0;
	virtual int release() = 0;
	virtual int handle_ipc_depend_on_status(IpcCommand cmd) = 0;
	virtual int handle_select_time_out();
	virtual int handle_network_states();
	virtual int run_parsing_command();
	virtual bool GoNextState();
	virtual MAINSTATES GetMainHandlerState() = 0;

protected:
	virtual int handle_factory_reset();

protected:
	IpcDaemon ipc_daemon;
	StatesHolder *holder;
	button_states keepButtonStates;
	LEDController led;
	MultiSender sender;
	MultiCastPackage pack;
};

#endif

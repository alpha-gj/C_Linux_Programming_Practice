#ifndef _IPCHANDLER_H_
#define _IPCHANDLER_H_
#include <unistd.h>
#include "ipc/ipc_cmd.h"
#include "sys/socket.h"
#include "todo.h"
#include "command.h"
#include "Common.h"
#include "StatesHolder.h"
#include "HwManager.h"

class IPCHandler
{
public:
	IPCHandler ();
	virtual ~IPCHandler ();
	virtual int init() = 0;
	virtual int release() = 0;
	virtual int run_parsing_command();
	virtual bool GoNextState();
	virtual MAINSTATES GetMainHandlerState() = 0;

protected:
	IpcDaemon ipc_daemon;
	StatesHolder *holder;
	int ipc_fd;
	int handle_factory_reset();
	int handle_detect_factory_button();
	int handle_stream_count(bool isActive);
};

#endif

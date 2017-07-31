#ifndef _REBOOTHANDLER_H_
#define _REBOOTHANDLER_H_
#include "IPCHandler.h"

class RebootHandler : public IPCHandler
{
public:
	RebootHandler ();
	virtual ~RebootHandler ();
	virtual int init();
	virtual int release();
	virtual int handle_ipc_depend_on_status(IpcCommand cmd);
	virtual MAINSTATES GetMainHandlerState();

private:
};

#endif

#ifndef _RESETHANDLER_H_
#define _RESETHANDLER_H_
#include "IPCHandler.h"

class ResetHandler : public IPCHandler
{
public:
	ResetHandler ();
	virtual ~ResetHandler ();
	virtual int init();
	virtual int release();
	virtual int handle_ipc_depend_on_status(IpcCommand cmd);
	virtual MAINSTATES GetMainHandlerState();

private:
};

#endif

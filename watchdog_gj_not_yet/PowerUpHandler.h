#ifndef _POWERUPHANDLER_H_
#define _POWERUPHANDLER_H_
#include "IPCHandler.h"

class PowerUpHandler : public IPCHandler
{
public:
	PowerUpHandler ();
	virtual ~PowerUpHandler ();
	virtual int init();
	virtual int release();
	virtual int handle_ipc_depend_on_status(IpcCommand cmd);
	virtual MAINSTATES GetMainHandlerState();

private:

private:
	void check_associate();
};

#endif

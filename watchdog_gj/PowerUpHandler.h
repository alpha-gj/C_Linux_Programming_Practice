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
	virtual MAINSTATES GetMainHandlerState();
private:
	int number;
};

#endif

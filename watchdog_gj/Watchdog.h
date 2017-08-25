#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include "StatesHolder.h"
#include "IPCHandler.h"

class Watchdog
{
public:
	Watchdog ();
	virtual ~Watchdog ();
	virtual int init();
	virtual int run();
	virtual int release();

protected:
	StatesHolder *holder;
	IPCHandler *CreateHandlerByStates();

private:
};

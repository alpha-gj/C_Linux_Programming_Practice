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
#include <iostream>
#include "StatesHolder.h"
#include "IPCHandler.h"
#include "Common.h"

class Watchdog
{
	public:
		Watchdog ();
		~Watchdog ();
		int run();
	private:
		StatesHolder *holder;
		IPCHandler *CreateHandlerByStates();
};

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
#include <iostream>
#include "watcher/watcher.h"
#include "Common.h"
#include "HwManager.h"
#include "StatesHolder.h"
#include "PowerUpHandler.h"
#include "IPCHandler.h"
#include "StatesHolder.h"

class Watchdog
{
	public:
		Watchdog ();
		~Watchdog ();
		int init();
		int run();
		int deinit();

	private:
		HwManager *hw_manager;
		StatesHolder *holder;
		IPCHandler *CreateHandlerByStates();
};

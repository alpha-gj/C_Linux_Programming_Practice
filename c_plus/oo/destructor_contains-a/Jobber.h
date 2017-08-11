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

#define AUDIO_ALER_MAX_DB 100

class Jobber
{
public:
	Jobber ();
#if 1
	virtual ~Jobber ();
	virtual int init();
	virtual int run();
	virtual int release();
#else
	~Jobber ();
	int init();   	
	int run();
	int release();
#endif

protected:
	StatesHolder *holder;
	IPCHandler *handler;
	IPCHandler *CreateHandlerByStates();
};

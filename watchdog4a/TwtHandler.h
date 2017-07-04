#ifndef _TWTHANDLER_H_
#define _TWTHANDLER_H_
#include "IPCHandler.h"

class TwtHandler : public IPCHandler
{
public:
	TwtHandler ();
	virtual ~TwtHandler ();
	virtual int init();
	virtual int release();
	virtual int handle_select_time_out();
	virtual int handle_ipc_depend_on_status(IpcCommand cmd);
	virtual MAINSTATES GetMainHandlerState();

private:
	int timeout_count;
	bool needStopCallingOut;
	pthread_t pid;

private:
	void changeColor();
	void enterNormal();
};

#endif

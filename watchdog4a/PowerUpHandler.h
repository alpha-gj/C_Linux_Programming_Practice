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
	virtual int handle_select_time_out();
	virtual int handle_network_states();
	virtual MAINSTATES GetMainHandlerState();

private:
	WPSSTATES ack;
	int waitHeartbeatNotifyCount;
	bool noWifiSetting;

private:
	int enterNormalBootup();
	void check_associate();
	static void * runWPSStartThread(void *args);
};

#endif

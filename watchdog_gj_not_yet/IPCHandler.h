#ifndef _IPCHANDLER_H_
#define _IPCHANDLER_H_
#include <unistd.h>
#include "ipc/ipc_cmd.h"
#include "sys/socket.h"
#include "wlan/wlan.h"
#include "sensor/sensor.h"
#include "MulCast/MulCast.h"
#include "str/str.h"
#include "todo.h"
#include "command.h"
#include "Common.h"
#include "StatesHolder.h"
#include "HwManager.h"

class IPCHandler
{
public:
	IPCHandler ();
	virtual ~IPCHandler ();
	virtual int init() = 0;
	virtual int release() = 0;
	virtual int run_parsing_command();
	virtual bool GoNextState();
	virtual MAINSTATES GetMainHandlerState() = 0;

protected:
	IpcDaemon ipc_daemon;
	StatesHolder *holder;
	HwManager *hw_manager;
	int ipc_fd;

	/* IPC EVENT */
	int handle_set_ir_mode_auto();
	int handle_set_ir_mode_off();
	int handle_set_ir_mode_on();
	int handle_factory_reset();
	int handle_detect_factory_button();
	int handle_stream_count(bool isActive);
	int handle_associated();
	int handle_deassociated();
	int handle_day_mode();
	int handle_night_mode();
	int handler_firmware_upgrade();
};

#endif

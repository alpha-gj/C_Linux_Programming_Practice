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
		virtual int deinit() = 0;
		int run_parsing_command();

		/* Get Event By Type */
		void DoEventByType(EVENT event);

	protected:
		IpcDaemon ipc_daemon;
		StatesHolder *holder;
		HwManager *hw_manager;
		int ipc_fd;

		/* Opreate States of Handler */
		MAINSTATES GetMainStates();
		MAINSTATES GetOldMainStates();
		void SetMainStates(MAINSTATES states);
		bool IsStatesChanged();
		
		

		/* IPC Event Common */
		int handle_factory_reset();
		int handle_detect_factory_button();
		int handle_firmware_upgrade();
		int handle_stream_count(bool isActive);
		int handle_update_thread_value();
		virtual int handle_ipc_depend_on_status(IpcCommand cmd) = 0;

		/* IPC Event ICR/IR */
		int handle_set_ir_mode_auto();
		int handle_set_ir_mode_off();
		int handle_set_ir_mode_on();
		int handle_day_mode();
		int handle_night_mode();

		/* IPC Event Network */
		int handle_associated();
		int handle_deassociated();
};

#endif

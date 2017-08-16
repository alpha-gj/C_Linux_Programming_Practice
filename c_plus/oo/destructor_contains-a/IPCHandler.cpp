#include "IPCHandler.h"
#include <iostream>
using namespace std;

IPCHandler::IPCHandler():holder(NULL)
{
	cout << "*** IPCHandler constructer without pra is running ... ***" << endl;
	/*
	do {
		if (ipc_daemon.init(WATCHDOG_SOCKET_NAME) < 0) {
			ERROR("create ipc daemon fail");
			break;
		}
		ret = 0;
	} while(false);
	led.init();
	*/
	holder = StatesHolder::CreateStatesHolder();
	if (holder == NULL) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

IPCHandler::~IPCHandler()
{
	cout << "*** IPCHandler destructer is running ... ***" << endl;
	if (holder) {
		StatesHolder::ReleaseStatesHolder();
		holder = NULL;
	}
}

int IPCHandler::init()
{
	int ret = 0;
	/*
	do {
		if (ipc_daemon.init(WATCHDOG_SOCKET_NAME) < 0) {
			ERROR("create ipc daemon fail");
			break;
		}
		ret = 0;
	} while(false);
	led.init();
	holder = StatesHolder::CreateStatesHolder();
	if (!holder) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}
	*/
	return ret;
}

int IPCHandler::release()
{
	/*
	fprintf(stderr, "%s: %s[%d] IPCHandler is released\n", __FILE__, __FUNCTION__, __LINE__);
	if (holder) {
		StatesHolder::releaseStatesHolder();
		holder = NULL;
	}
	*/
	return 0;
}

bool IPCHandler::GoNextState()
{
	//return (GetMainHandlerState() != holder->GetMainStates());
	return 0;
}

int IPCHandler::run_parsing_command()
{
	/*
	int client_fd, ret;
	IpcCommand cmd;
	while (!get_quit() && !get_reload() && !GoNextState()) {
		//INFO("%d %d", get_quit(), get_reload());
		ret = ipc_daemon.wait_cmd_timeout(&cmd, sizeof(cmd), client_fd, 0, POLLING_TIMEOUT);
		if (ret > 0) {
			IpcResponse res = { 0, 0, 0, "" };	// always response with success.
			INFO("\033[1;33m[watchDog]: received command[%d]\033[0m", cmd.id);
			switch (cmd.id) {
				case CMD_FACTORY_RESET_ACT:
					res.result = handle_factory_reset();
					break;
				case CMD_FIRMWARE_UPGRADE_START: {
													 Color green(LED_GREEN, BLINK, FIXED_VALUE_FOR_INTERACTIVE, BLINK_PERIOD);
													 Color yellow(LED_YELLOW, BLINK, FIXED_VALUE_FOR_INTERACTIVE, BLINK_PERIOD);
													 led.setBlink(green, yellow);
													 res.result = 0;
												 } break;
				case CMD_FIRMWARE_UPGRADE_DONE: {
													Color color(LED_OFF, OFF);
													led.setColor(color);
													res.result = 0;
												} break;
				case CMD_ENTER_DAY_MODE:
				case CMD_ENTER_NIGHT_MODE:
				case CMD_STANDBY_LED_UPDATE: {
												 Color c = holder->GetLastColor1();
												 Color n(c.pattern, c.mode, c.interactive, c.period, c.brightness);
												 led.setColor(n);
												 res.result = 0;
											 } break;
				case CMD_DEVICE_CONNECT:
											 holder->SetPanelACK(true);
											 res.result = 0;
											 break;
				case CMD_DEVICE_DISCONNECT:
											 holder->SetPanelACK(false);
											 res.result = 0;
											 break;
				case CMD_ASSOCIATED:
				case CMD_LINK_UP:
											 holder->SetAssociated(true);
											 res.result = 0;
											 break;
				case CMD_DEASSOCIATED:
				case CMD_LINK_DOWN:
											 holder->SetAssociated(false);
											 res.result = 0;
											 break;
				case CMD_SET_ACTIVE:
											 res.result = 0;
											 break;
				case CMD_DET_BUTTON:
											 res.value = holder->GetButtonStates().buttonOn;
											 break;
				default:
											 res.result = handle_ipc_depend_on_status(cmd);
											 break;
			}
			ipc_daemon.reply_cmd(client_fd, &res, sizeof(res));
		} else if (ret == 0) {
			handle_select_time_out();
			handle_network_states();
		}
	}
	return 0;
	*/
}

int IPCHandler::handle_factory_reset()
{
	return 0;
}

int IPCHandler::handle_select_time_out()
{
	return 0;
}

int IPCHandler::handle_network_states()
{
	return 0;
}


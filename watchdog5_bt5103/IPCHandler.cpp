#include "IPCHandler.h"
#include <iostream>
using namespace std;

IPCHandler::IPCHandler():holder(NULL),hw_manager(NULL)
{
	int	ret = -1;
	cout << "*** IPCHandler constructer without pra is running ... ***" << endl;
	ipc_fd = ipc_daemon.init(WATCHDOG_SOCKET_NAME);

	cout << "ipc_fd is " << ipc_fd << endl;
	do {
		if (ipc_fd < 0) {
			ERROR("create ipc daemon fail");
			break;
		}
		ret = 0;
	} while(false);

	holder = StatesHolder::CreateStatesHolder();
	if (holder == NULL) {
		fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
	}

	hw_manager = HwManager::CreateHwManager();
	if (hw_manager == NULL) {
		fprintf(stderr, "%s: %s[%d] can not alloc hw_manager\n", __FILE__, __FUNCTION__, __LINE__);
	}
}

IPCHandler::~IPCHandler()
{
	cout << "*** IPCHandler destructer is running ... ***" << endl;
	cout << "ipc_fd is " << ipc_fd << endl;

	if (hw_manager) {
		HwManager::ReleaseHwManager();
		hw_manager = NULL;
	}
	if (holder) {
		StatesHolder::ReleaseStatesHolder();
		holder = NULL;
	}

	//TODO It should be closed by ipc_daemon.deinit, but ipc_damemon didin't implement this function.
	//close(ipc_fd); <--- need it? ls /proc/$(pidof Watchdog)/fd to watch it.
}

int IPCHandler::init()
{
	return 0;
}

int IPCHandler::deinit()
{
	return 0;
}

MAINSTATES IPCHandler::GetMainStates()
{
	return holder->GetMainStates();
}

MAINSTATES IPCHandler::GetOldMainStates()
{
	return holder->GetOldMainStates();
}

void IPCHandler::SetMainStates(MAINSTATES states)
{
	holder->SetMainStates(states);
}

bool IPCHandler::IsStatesChanged()
{
	return holder->IsStatesChanged();
}

int IPCHandler::run_parsing_command()
{
	int client_fd, ret;
	IpcCommand cmd;
	while (!get_quit() && !get_reload() && !IsStatesChanged()) {
		//INFO("%d %d", get_quit(), get_reload());
		ret = ipc_daemon.wait_cmd_timeout(&cmd, sizeof(cmd), client_fd, 0, 0);
		if (ret > 0) {
			IpcResponse res = { 0, 0, 0, "" };	// always response with success.
			INFO("\033[1;33m[watchDog]: received command[%d]\033[0m", cmd.id);
			switch (cmd.id) {
				case CMD_IR_AUTO:
					fprintf(stderr, "CMD_IR_AUTO\n");
					res.result = handle_set_ir_mode_auto();
					break;
				case CMD_IR_OFF:
					fprintf(stderr, "CMD_IR_OFF\n");
					res.result = handle_set_ir_mode_off();
					break;
				case CMD_IR_ON:
					fprintf(stderr, "CMD_IR_ON\n");
					res.result = handle_set_ir_mode_on();
					break;
				case CMD_DET_FACTORY_BUTTON:
					fprintf(stderr, "DET_FACTORY_BUTTON\n");
					res.result = handle_detect_factory_button();
					break;
				case CMD_FACTORY_RESET_ACT: 
					fprintf(stderr, "FACTORY_RESET_ACT\n");
					res.result = handle_factory_reset();
					break;
				case CMD_SET_ACTIVE:
					fprintf(stderr, "CMD_SET_ACTIVE\n");
					if(cmd.action == 0) { /* streaming is active */
						res.result = handle_stream_count(true);
					} else {			  /* streaming is inactive */
						res.result = handle_stream_count(false);
					}
					break;
				case CMD_ASSOCIATED:
					fprintf(stderr, "CMD_ASSOCIATED\n");
					res.result = handle_associated();
					break;
				case CMD_DEASSOCIATED:
					fprintf(stderr, "CMD_DEASSOCIATED\n");
					res.result = handle_deassociated();
					/* TODO: And then, associ_check will handle this behavior, such as enable AP Mode or not */
					break;
				case CMD_ENTER_DAY_MODE:
					fprintf(stderr, "CMD_ENTER_DAY_MODE\n");
					res.result = handle_day_mode();
					break;
				case CMD_ENTER_NIGHT_MODE:
					fprintf(stderr, "CMD_ENTER_NIGHT_MODE\n");
					res.result = handle_night_mode();
					break;
				case CMD_NETWORK_RESTART:
					fprintf(stderr, "CMD_NETWORK_RESTART\n");
					system("/etc/rc.d/init.d/network.sh reload &");
					res.result = 0;
					break;
				case CMD_BLUEZ_STOP:
					fprintf(stderr, "CMD_BLUEZ_STOP\n");
					system("/etc/rc.d/init.d/bluetoothd.sh stop &");
					res.result = 0;
					break;
				case CMD_FIRMWARE_UPGRADE_START:
					fprintf(stderr, "FIRMWARE_UPGRADE_START\n");
					res.result = handle_firmware_upgrade();
					break;
				case CMD_RELOAD_CONFIG:
					fprintf(stderr, "CMD_RELOAD_CONFIG\n");
					res.result = handle_update_thread_value();
					break;
				default:
					res.result = handle_ipc_depend_on_status(cmd);
				break;
			}

			ipc_daemon.reply_cmd(client_fd, &res, sizeof(res));
		} else if (ret == 0) {
			/* Do Nothing */		
		}
	}
	return 0;
}

int IPCHandler::handle_set_ir_mode_auto()
{
	LIGHT_SENSOR_STATUS_SETTING light_sensor_status_setting;

	/* It should get current Light Sensor_state First */ 
	holder->get_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	/* Set only one of settings that you want to do */
	light_sensor_status_setting.ir_mode = IR_AUTO;
	holder->set_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	return 0;
}

int IPCHandler::handle_set_ir_mode_off()
{
	LIGHT_SENSOR_STATUS_SETTING light_sensor_status_setting;

	/* It should get current Light Sensor_state First */ 
	holder->get_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	/* Set only one of settings that you want to do */
	light_sensor_status_setting.ir_mode = IR_MANUAL;
	light_sensor_status_setting.ir_led_state = IR_LED_OFF;
	light_sensor_status_setting.old_ir_led_state = IR_LED_OFF;
	holder->set_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	return handle_day_mode();
}

int IPCHandler::handle_set_ir_mode_on()
{
	LIGHT_SENSOR_STATUS_SETTING light_sensor_status_setting;

	/* It should get current Light Sensor_state First */ 
	holder->get_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	/* Set only one of settings that you want to do */
	light_sensor_status_setting.ir_mode = IR_MANUAL;
	light_sensor_status_setting.ir_led_state = IR_LED_ON;
	light_sensor_status_setting.old_ir_led_state = IR_LED_ON;
	holder->set_status_info_by_type("LightSensorStatus", &light_sensor_status_setting);

	return handle_night_mode();
}

int IPCHandler::handle_detect_factory_button()
{
	LED_STATUS_SETTING led_status_setting;

	/* It should get current LED_state First */ 
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);

	/* Set only one of settings that you want to do */
	if (led_status_setting.pled_state != PLED_RESET && (led_status_setting.pled_state = PLED_RESET)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}

	return 0;
}

int IPCHandler::handle_factory_reset()
{
	system("( /etc/rc.d/init.d/services.sh stop; factoryReset > /dev/null 2> /dev/null; /sbin/reboot ) &");
	
	return 0;
}

int IPCHandler::handle_stream_count(bool isActive)
{
	static unsigned count = 0;
	
	if (isActive) {
		count++;
	} else if (!isActive && count > 0) {
		count--;
	} else {
		INFO("BUG@%d", __LINE__);
	}
	
	fprintf(stderr, "%s: %s[%d] Live View count is %d\n", __FILE__, __FUNCTION__, __LINE__, count);

	/* Set LED State by count */
	LED_STATUS_SETTING led_status_setting;
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if (count > 0) {
		if(led_status_setting.pled_state != PLED_ACTIVE && (led_status_setting.pled_state = PLED_ACTIVE)) {
			holder->set_status_info_by_type("LEDStatus", &led_status_setting);
		}
	} else if (count == 0) {
		NETWORK_STATUS_SETTING network_status_setting;
		holder->get_status_info_by_type("NetworkStatus", &network_status_setting);
		
		switch (network_status_setting.link_state) {
			case LINK_IS_OFF:
				printf("LINK_IS_OFF");
				if(led_status_setting.pled_state != PLED_BT_MODE && (led_status_setting.pled_state = PLED_BT_MODE)) {
					printf("set it\n");
					holder->set_status_info_by_type("LEDStatus", &led_status_setting);
				}
				break;
			case LINK_IS_ON:
				printf("LINK_IS_ON");
				if(led_status_setting.pled_state != PLED_CLIENT_MODE && (led_status_setting.pled_state = PLED_CLIENT_MODE)) {
					printf("set it\n");
					holder->set_status_info_by_type("LEDStatus", &led_status_setting);
				}
				break;
		}
	} else {
		INFO("BUG@%d", __LINE__);
	}

	return 0;
}


int IPCHandler::handle_associated() 
{
	/* pled state is PLED_CLIENT_MODE */
	LED_STATUS_SETTING led_status_setting;
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if(led_status_setting.pled_state != PLED_CLIENT_MODE && (led_status_setting.pled_state = PLED_CLIENT_MODE)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}

	/* TODO wled state follow NetworkStatus. The wled state does nothing here, but we can set WLED_STRONG first */
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if(led_status_setting.wled_state != WLED_STRONG && (led_status_setting.wled_state = WLED_STRONG)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}

	return 0;
}

int IPCHandler::handle_deassociated()
{
	/* pled state is PLED_BT_MODE(default) or PLED_AP_MODE */
	LED_STATUS_SETTING led_status_setting;

	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if(led_status_setting.pled_state != PLED_BT_MODE  && (led_status_setting.pled_state = PLED_BT_MODE)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}

	/* wled_state get WLED_WEAK by using led_status in IPCHandler */
	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if(led_status_setting.wled_state != WLED_WEAK && (led_status_setting.wled_state = WLED_WEAK)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}

	return 0;
}

int IPCHandler::handle_day_mode()
{
	MultiSender ceSender;
	MultiCastPackage trigger;
	IRLED_SETTING irled_setting {
		.id = AHAL_PWM_ID_IR_LED,
		.value_type = AHAL_PWM_PERCENT,
		.value = 0
	};
	ICR_SETTING icr_setting {
		.state = AHAL_CST_STATE_OFF
	};
	
	sensor_update_para(IMAGE_BW, 0);
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);

	INFO("IR State = %d\n", 0);
	echo(WATCHDOG_UPDATE_IR_STATE_FILE, string("0"));
	trigger.iOnOff = 0;
	ceSender.Init(DEFAULT_CHANNEL);
	trigger.iEventMask = IR_LED_ON_OFF_EVENT;
	ceSender.SendPackage(&trigger);

	return 0;
}

int IPCHandler::handle_night_mode()
{
	MultiSender ceSender;
	MultiCastPackage trigger;
	IRLED_SETTING irled_setting {
		.id = AHAL_PWM_ID_IR_LED,
		.value_type = AHAL_PWM_PERCENT,
		.value = PWD_PERCENTAGE
	};
	ICR_SETTING icr_setting {
		.state = AHAL_CST_STATE_ON
	};

	sensor_update_para(IMAGE_BW, 1);
	hw_manager->set_hw_info_by_type("IRLED", &irled_setting);
	hw_manager->set_hw_info_by_type("ICR", &icr_setting);

	INFO("IR State = %d\n", 1);
	echo(WATCHDOG_UPDATE_IR_STATE_FILE, string("1"));
	ceSender.Init(DEFAULT_CHANNEL);
	trigger.iEventMask = IR_LED_ON_OFF_EVENT;
	ceSender.SendPackage(&trigger);

	return 0;
}

int IPCHandler::handle_firmware_upgrade()
{
	/* pled state is PLED_BT_MODE */
	LED_STATUS_SETTING led_status_setting;

	holder->get_status_info_by_type("LEDStatus", &led_status_setting);
	if(led_status_setting.pled_state != PLED_FWUPDATE  && (led_status_setting.pled_state = PLED_FWUPDATE)) {
		holder->set_status_info_by_type("LEDStatus", &led_status_setting);
	}
	return 0;
}

int IPCHandler::handle_update_thread_value()
{
	holder->update_thread_value_by_type("LightSensorStatus");
	holder->update_thread_value_by_type("LEDStatus");

	return 0;
}

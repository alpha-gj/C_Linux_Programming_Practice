#ifndef _LIGHTSENSORSTATUS_H_
#define _LIGHTSENSORSTATUS_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cam/cam.h"
#include "MulCast/MulCast.h"
#include "Common.h"
#include "SwStatus.h"
#include "LightSensorController.h"
#include "IRLEDController.h"
#include "ICRController.h"

using namespace std;

#define TIME_INTERVAL		500000
#define DAY_TO_NIGHT_SEC	3 * (1000000/TIME_INTERVAL)
#define NIGHT_TO_DAY_SEC	7 * (1000000/TIME_INTERVAL)
#define MIN_threshold 50

typedef struct LIGHT_SENSOR_SETTING_FROM_CAM {
	ir_setting ir_set;
	lightsensor_setting lightsensor_set;
} LIGHT_SENSOR_SETTING_FROM_CAM;	/* It should get settings from DB first */

typedef enum {
	IR_AUTO,		/* By LightSensor to control IRLED/ICR */
	IR_MANUAL,		/* By MANUAL to control IRLED/ICR */	
} IR_MODE;		

typedef enum {
	IR_LED_OFF,		/* For Day Mode  */
	IR_LED_ON,		/* For Night mode */
} IR_LED_STATE;		

typedef struct LIGHT_SENSOR_STATUS {
	IR_MODE ir_mode;
	IR_LED_STATE ir_led_state;
	IR_LED_STATE old_ir_led_state;
	int threshold;
	int D2N_offset;	 
	int N2D_offset;	 
} LIGHT_SENSOR_STATUS_SETTING;

typedef struct IRLED_ICR {
	IRLED_SETTING irled_setting;
	ICR_SETTING icr_setting;
} IRLED_ICR_SETTING;



class LightSensorStatus : public SwStatus
{
	private:
		pthread_t light_sensor_status_pid;
		static void *run_light_sensor_status_detect_thread(void *args);
		static bool isPauseDetect;
		static LIGHT_SENSOR_STATUS_SETTING s_light_sensor_status_setting;
		
		/* Function*/
		static IR_LED_STATE get_IR_MODE_by_cam_settings(LIGHT_SENSOR_SETTING_FROM_CAM light_sensor_setting_from_cam);
		static int set_ir_icr_behavior_by_state(IR_LED_STATE ir_led_state);

	public:
		LightSensorStatus();
		virtual ~LightSensorStatus();
		virtual int init();								
		virtual int deinit();
		virtual int run_status_detect();	
		virtual int pause_status_detect();
		virtual int continue_status_detect();
		virtual bool get_pause_detect_flag();
		virtual int set_status_info(void *status_struct = NULL);
		virtual int get_status_info(void *status_struct = NULL);
};

#endif
	




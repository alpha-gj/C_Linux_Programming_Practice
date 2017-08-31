#define BUTTON_POLLING_TIME     50      //50ms
#define POLLING_TIMEOUT         100000  //100ms
#define TIME_BASE               (1000000/POLLING_TIMEOUT)
#define WAIT_HEART_BEAT_COUNT	(120 * TIME_BASE)
#define ENTER_TWT_COUNT			(0 * TIME_BASE)
#define ENTER_NORMAL_COUNT		(3 * TIME_BASE)
#define ENTER_REBOOT_COUNT		(10 * TIME_BASE)
#define ENTER_STANDBY_COUNT		(20 * TIME_BASE)
#define ENTER_REPORT_COUNT             (60 * TIME_BASE)
#define ENTER_RESET_COUNT		(30 * TIME_BASE)
#define ENTER_WPS_COUNT			(3 * TIME_BASE)
#define WAIT_CALL_OUT_COUNT		(45 * TIME_BASE)
#define GPIO_READ_FAIL_COUNT		(3 * TIME_BASE)
#define WAIT_WPS_STATE_COUNT		(1 * TIME_BASE)
#define WAIT_CONNECT_TO_DEVICE		(120 * TIME_BASE)
#define NETWORK_RESTART_TIMEOUT 	(10 * TIME_BASE)
#define WPS_PBC_SCAN_COUNT			(90 * TIME_BASE)

#define SOLID_PERIOD				400
#define BREATHING_PERIOD				1700
#define BLINK_PERIOD					750
#define QUICK_FLICKER_PERIOD			158

#define MAX_GEO_INIT_RETRY	5
#define BUTTON_GPIO_NUM		2
#define MAX_GPIO_NUM		25

#define PLAY_INITIALING_START_VOICE	"/usr/share/chimes/16k1ch_01-initialization-3A.wav"
#define PLAY_BOOTINGUP_TONE		"/usr/share/chimes/16k1ch_02-progress-3J.wav"
#define PLAY_BOOTINGUP_VOICE		"/usr/share/chimes/16k1ch_02-voice1-camera-intro.wav"
#define SETUP_CONNECT_FAIL_VOICE	"/usr/share/chimes/16k1ch_06-voice11-unable-to-connect-ERROR.wav"
#define WPS_BEGIN_TONE			"/usr/share/chimes/16k1ch_03-now-pairing-3L.wav"
#define PLAY_PAIRED_END_VOICE		"/usr/share/chimes/16k1ch_04-voice8-camera-connected-now-finalizing.wav"
#define PLAY_PAIRING_START_VOICE	"/usr/share/chimes/16k1ch_03-voice7-now-connecting.wav"
#define PLAY_FINALIZING_END_VOICE	"/usr/share/chimes/16k1ch_05-voice10-success.wav"
#define PLAY_TWT_START_TONE		"/usr/share/chimes/16k1ch_04-progress-3J.wav"
#define PLAY_TWT_BEGIN_TONE		"/usr/share/chimes/16k1ch_06-sign-on-3P.wav"
#define PLAY_TWT_END_TONE		"/usr/share/chimes/16k1ch_07-sign-off-3Q.wav"
#define AEC_RESET			"{ mixer aec_rst; } > /dev/null 2> /dev/null"
#define NAME_CALL_OUT_TONE			"callout"
#define CALL_OUT_TONE				"/usr/share/chimes/16k1ch_08-twt-callout.wav"
#define LOAD_CALL_OUT_TONE			"pacmd load-sample " NAME_CALL_OUT_TONE " " CALL_OUT_TONE
#define PLAY_CALL_OUT_TONE				"pacmd play-sample " NAME_CALL_OUT_TONE " 0"

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
#include "StatesHolder.h"
#include "IPCHandler.h"

#define AUDIO_ALER_MAX_DB 100

class Jobber
{
public:
	Jobber ();
	virtual ~Jobber ();
	virtual int init();
	virtual int run();
	virtual int release();
	int start_audio_alert(struct audio_det_struct &audio_det);
	int stop_audio_alert();
	struct audio_det_struct det;

public:
	void setup_audio_alert_threshold(struct audio_det_struct &det);

protected:
	StatesHolder *holder;
	IPCHandler *CreateHandlerByStates();

private:
	int start_button_notification();
	int stop_button_notification();
private:
	bool need_clean_notify;
	static void aalarm_callback(audio_alert_info * audalert_info, void * user_data);
};

#ifndef _AUDIOCONTROLLER_H_
#define _AUDIOCONTROLLER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "HwController.h"

using namespace std;

typedef struct AUDIO
{	
	AHAL_INT32 value;
} AUDIO_SETTING;

class AudioController : public HwController
{
	public:
		AudioController();
		virtual ~AudioController();
		virtual int init();
		virtual int deinit();
		virtual	int set_hw_info(void* hw_struct = NULL);
		virtual	int get_hw_info(void* hw_struct = NULL);
};
#endif
	




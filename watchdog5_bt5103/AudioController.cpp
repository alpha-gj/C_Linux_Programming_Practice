#include <iostream>
#include "AudioController.h"

AudioController::AudioController()
{
	/* Do nothing */
}

AudioController::~AudioController()
{
	/* Do nothing */
}

int AudioController::init()
{
	int ret = AHAL_RET_FAIL;

	system("amixer cset numid=3,iface=MIXER,name='PGA Volume' 33 > /dev/null");
	system("amixer cset numid=4,iface=MIXER,name='Input Select' 2 > /dev/null");
	ret = AHAL_AUDIO_Init(NULL);
	AHAL_AUDIO_SetSpeaker(AHAL_CST_STATE_ON);

	fprintf(stderr, "Audio init\n");
	return (int)ret;
}

int AudioController::deinit()
{
	fprintf(stderr, "Audio deinit\n");
	return (int)AHAL_AUDIO_UnInit();
}


int AudioController::set_hw_info(void *) 
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

int AudioController::get_hw_info(void *)
{
	return (int)AHAL_RET_NOT_SUPPORT;
}

#include "PowerUpHandler.h"

PowerUpHandler::PowerUpHandler()
{
	printf("*** PowerUpHandler construter is running... ***\n");
}

PowerUpHandler::~PowerUpHandler()
{
	printf("*** PowerUpHandler destruter is running... ***\n");
}

int PowerUpHandler::init()
{
	
	if (IPCHandler::init()) {
		return -1;
	} else {
		/*
		Color white(LED_WHITE, SOLID, FIXED_VALUE_FOR_INTERACTIVE, SOLID_PERIOD);
		led.setColor(white);
		ack = holder->GetWpsStates();
		keepButtonStates = holder->GetButtonStates();
		if (keepButtonStates.buttonOn) {
			holder->SetMainStates(POWERUPWITHBUTTON);
		} else {
			enterNormalBootup();
		}
		*/
		return 0;
	}
	

	return 0;
}

int PowerUpHandler::release()
{
	IPCHandler::release();
	return 0;
}

MAINSTATES PowerUpHandler::GetMainHandlerState()
{
	return POWERUP;
}

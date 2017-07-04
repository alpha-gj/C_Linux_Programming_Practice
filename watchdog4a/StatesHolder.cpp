#include "StatesHolder.h"

#include <cam/cam.h>

static StatesHolder *holder = NULL;
static int referCount = 0;

StatesHolder *StatesHolder::CreateStatesHolder()
{
	if (holder == NULL) {
		holder = new StatesHolder();
	}
	if (holder) {
		++referCount;
	}
	return holder;
}

void StatesHolder::releaseStatesHolder()
{
	if (holder) {
		if (referCount <=  0) {
			delete holder;
			holder = NULL;
		} else {
			--referCount;
		}
	}
}

StatesHolder::StatesHolder()
{
	mainStates = POWERUP;
	oldMainStates = mainStates;
	pthread_mutex_init(&mainStatesLock, NULL);
}

StatesHolder::~StatesHolder()
{
	pthread_mutex_destroy(&mainStatesLock);
}

void StatesHolder::SetMainStates(MAINSTATES s)
{
	pthread_mutex_lock(&mainStatesLock);
	oldMainStates = mainStates;
	mainStates = s;
	pthread_mutex_unlock(&mainStatesLock);
}

MAINSTATES StatesHolder::GetMainStates()
{
	pthread_mutex_lock(&mainStatesLock);
	MAINSTATES t = mainStates;
	pthread_mutex_unlock(&mainStatesLock);
	return t;
}

MAINSTATES StatesHolder::GetOldMainStates()
{
	pthread_mutex_lock(&mainStatesLock);
	MAINSTATES t = oldMainStates;
	pthread_mutex_unlock(&mainStatesLock);
	return t;
}


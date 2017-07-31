#ifndef _STATESHOLDER_H_
#define _STATESHOLDER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Color.h"
#include "tinydb/tinydb.h"
#include "cam/cam.h"
#include "todo.h"

enum MAINSTATES {
	POWERUP,
	SETUP,
	CONNECTED,
	DISCONNECTED,
	RESET,
	REBOOT,
	MAINSTATES_COUNT,
};

// TODO, ???
struct net_states {
	bool needHandleAssociated;
	bool associated;
};

class StatesHolder
{
public:
	virtual ~StatesHolder ();
	static StatesHolder *CreateStatesHolder();
	static void releaseStatesHolder();

public:
	void SetMainStates(MAINSTATES s);
	void SetMainStatesByCameraMode();
	MAINSTATES GetMainStates();
	MAINSTATES GetOldMainStates();

private:
	StatesHolder ();

private:
	pthread_mutex_t mainStatesLock;
	MAINSTATES mainStates;
	MAINSTATES oldMainStates;

};

#endif

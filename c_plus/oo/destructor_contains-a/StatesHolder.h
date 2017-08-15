#ifndef _STATESHOLDER_H_
#define _STATESHOLDER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

enum MAINSTATES {
	POWERUP,
	MAINSTATES_COUNT,
};

class StatesHolder
{

public:
	static int referCount;

public:
	static StatesHolder *CreateStatesHolder();
	static int releaseStatesHolder();

public:
	void SetMainStates(MAINSTATES s);
	MAINSTATES GetMainStates();
	MAINSTATES GetOldMainStates();

private:
	StatesHolder();
	StatesHolder(const StatesHolder&);
	StatesHolder& operator=(const StatesHolder&);
	static StatesHolder *holder;
	virtual ~StatesHolder();

private:

	//pthread_mutex_t mainStatesLock;
	MAINSTATES mainStates;
	MAINSTATES oldMainStates;

};

#endif

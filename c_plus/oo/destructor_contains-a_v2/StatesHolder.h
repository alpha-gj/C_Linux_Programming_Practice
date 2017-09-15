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
	static int ReleaseStatesHolder();

public:
	void SetMainStates(MAINSTATES s);
	MAINSTATES GetMainStates();
	MAINSTATES GetOldMainStates();

	int GetStatusByComponent(const *char);

private:
	StatesHolder();
	StatesHolder(const StatesHolder&);
	StatesHolder& operator=(const StatesHolder&);
	~StatesHolder();
	bool CreateAllComponet();
	static StatesHolder *holder;

private:
	//pthread_mutex_t mainStatesLock;
	MAINSTATES mainStates;
	MAINSTATES oldMainStates;

};

#endif

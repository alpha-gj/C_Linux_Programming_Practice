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

private:
	StatesHolder();
	~StatesHolder();

	/* Stop copy constructor*/
	StatesHolder(const StatesHolder&);
	StatesHolder& operator=(const StatesHolder&);

	/* Follow Singleton Pattern */
	static StatesHolder *holder;
	static int referCount;

	//pthread_mutex_t mainStatesLock;
	MAINSTATES mainStates;
	MAINSTATES oldMainStates;

public:
	/* Follow Singleton Pattern */
	static StatesHolder *CreateStatesHolder();
	static int ReleaseStatesHolder();

	void SetMainStates(MAINSTATES s);
	MAINSTATES GetMainStates();
	MAINSTATES GetOldMainStates();

};

#endif

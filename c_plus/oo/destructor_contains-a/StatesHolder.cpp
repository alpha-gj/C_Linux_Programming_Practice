#include "StatesHolder.h"
#include <iostream>
using namespace std;

static StatesHolder *holder = NULL;
static int referCount = 0;

StatesHolder *StatesHolder::CreateStatesHolder()
{
	if (holder == NULL) {
		holder = new StatesHolder();
	}
	if (holder) {
		++referCount;
		printf("refer Count is %d\n", referCount);
	}
	return holder;
}

void StatesHolder::releaseStatesHolder()
{
	
	if (holder) {
		fprintf(stderr, "%s: %s[%d] holder is delete, referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
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
	cout << "*** StatesHolder constructer without pra is running ... ***" << endl;
	mainStates = POWERUP;
	oldMainStates = mainStates;
}

StatesHolder::~StatesHolder()
{
	cout << "*** StatesHolder destructer is running ... ***" << endl;
}

void StatesHolder::SetMainStates(MAINSTATES s)
{
	oldMainStates = mainStates;
	mainStates = s;
}

MAINSTATES StatesHolder::GetMainStates()
{
	MAINSTATES t = mainStates;
	return t;
}

MAINSTATES StatesHolder::GetOldMainStates()
{
	MAINSTATES t = oldMainStates;
	return t;
}


#include "StatesHolder.h"
#include <iostream>
using namespace std;

StatesHolder *StatesHolder::holder = NULL;
int StatesHolder::referCount = 0;

StatesHolder *StatesHolder::CreateStatesHolder()
{
	if (holder == NULL)  {
		holder = new StatesHolder();
	}
	if (holder) {
		++referCount;
		printf("refer Count is %d\n", referCount);
	}
	return holder;
}

int StatesHolder::ReleaseStatesHolder()
{
	bool ret = false;
	do {

		if (holder == NULL || referCount == 0) {
			fprintf(stderr, "%s: %s[%d] holder has been NULL or referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
			break;
		}

		if (referCount > 0) {
			--referCount;
			fprintf(stderr, "%s: %s[%d] referCount is %d\n", __FILE__, __FUNCTION__, __LINE__, referCount);
		}

		if (referCount == 0) {
			delete holder;
			holder = NULL;
			fprintf(stderr, "%s: %s[%d] holder is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		}

		ret = true;

	} while(false);

	return ret;
}

StatesHolder::StatesHolder():mainStates(POWERUP),oldMainStates(POWERUP)
{
	cout << "*** StatesHolder constructer without pra is running ... ***" << endl;
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


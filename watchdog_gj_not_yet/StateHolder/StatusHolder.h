#ifndef _STATUSHOLDER_H_
#define _STATUSHOLDER_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

class StatusHolder
{

public:
	static StatusHolder *CreateStatusHolder();
	static int ReleaseStatusHolder();

public:
	int SetStatus();

private:
	StatusHolder()a;
	StatusHolder(const StatusHolder&);
	StatusHolder& operator=(const StatusHolder&);
	~StatusHolder();
	static StatusHolder *statusholder;

private:


};

#endif

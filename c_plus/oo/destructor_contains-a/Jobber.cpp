#include "Jobber.h"
#include "PowerUpHandler.h"
#include "StatesHolder.h"
using namespace std;
Jobber *job = NULL;
bool get_quit = false;
bool get_reload = false;

Jobber::Jobber()
{
	cout << "*** Jobber constructer without pra is running ... ***" << endl;
}

Jobber::~Jobber()
{
	//make sure source is free or not?
	// check holder or handler
	cout << "*** Jobber destructer is running ... ***" << endl;
}

int Jobber::init()
{
	int ret = -1;
	do {
		holder = StatesHolder::CreateStatesHolder();
		if (!holder) {
			fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
			break;
		}
		ret = 0;
	} while(false);
	return ret;
}

int Jobber::run()
{
		IPCHandler *handler = CreateHandlerByStates();
		if (handler) {
			fprintf(stderr, "%s: %s[%d] handler create is OK\n", __FILE__, __FUNCTION__, __LINE__);
			if (!handler->init()) {
				fprintf(stderr, "%s: %s[%d] handler init is OK\n", __FILE__, __FUNCTION__, __LINE__);
#if 0				
				fprintf(stderr, "\n%s: %s[%d] handler->run_parsing_command()\n\n", __FILE__, __FUNCTION__, __LINE__);
				handler->run_parsing_command();
#else
				fprintf(stderr, "%s: %s[%d] handler->run_parsing_command()\n", __FILE__, __FUNCTION__, __LINE__);
				sleep(3);
#endif
			}
			handler->release();
			fprintf(stderr, "%s: %s[%d] handler is deleted\n", __FILE__, __FUNCTION__, __LINE__);
			delete handler;
		} else {
			fprintf(stderr, "%s: %s[%d] handler create is fail\n", __FILE__, __FUNCTION__, __LINE__);
			sleep(1);
		}

	return 0;
}

int Jobber::release()
{
	fprintf(stdout, "%s: %s[%d] Jobber is released\n", __FILE__, __FUNCTION__, __LINE__);
	if (holder) {
		StatesHolder::releaseStatesHolder();
	}
	return 0;
}

IPCHandler *Jobber::CreateHandlerByStates()
{
	switch (holder->GetMainStates()) {
		case POWERUP:
			fprintf(stdout, "%s: %s[%d] enter POWERUP\n", __FILE__, __FUNCTION__, __LINE__);
			return new PowerUpHandler();
			break;
		default:
			fprintf(stderr, "%s: %s[%d] BUG\n", __FILE__, __FUNCTION__, __LINE__);
			//return new PowerUpHandler();
			break;
	}
	return NULL;
}
/****************************************************
 *
 *
 *		main function
 *
 *
 */

int main(int /*argc*/, const char *argv[])
{
	job = new Jobber();
	if (job) {
		if (!job->init()) {
			job->run();
		} else {
			sleep(1);
		}
		job->release();
		fprintf(stderr, "%s: %s[%d] jobber is deleted\n", __FILE__, __FUNCTION__, __LINE__);
		delete job;
	} else {
		fprintf(stderr, "%s: %s[%d] can not alloc memory for job\n", __FILE__, __FUNCTION__, __LINE__);
		sleep(1);
	}
	return EXIT_SUCCESS;
}


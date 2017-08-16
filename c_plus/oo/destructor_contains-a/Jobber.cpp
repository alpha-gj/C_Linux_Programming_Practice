#include "Jobber.h"
#include "PowerUpHandler.h"
#include "StatesHolder.h"
using namespace std;
Jobber *job = NULL;
bool get_quit = false;
bool get_reload = false;

Jobber::Jobber():holder(NULL)
{
	cout << "*** Jobber constructer without pra is running ... ***" << endl;
		holder = StatesHolder::CreateStatesHolder();
		if (!holder) {
			fprintf(stderr, "%s: %s[%d] can not alloc holder\n", __FILE__, __FUNCTION__, __LINE__);
		}
}

Jobber::~Jobber()
{
	//make sure source is free or not?
	// check holder or handler
	cout << "*** Jobber destructer is running ... ***" << endl;
	if (holder) {
		StatesHolder::ReleaseStatesHolder();
		holder = NULL;
	}
}

int Jobber::init()
{
	fprintf(stdout, "%s: %s[%d] Jobber is inited\n", __FILE__, __FUNCTION__, __LINE__);
	int ret = -1;
	//TODO
	ret = 0;
	return ret;
}

int Jobber::run()
{
	IPCHandler *handler = CreateHandlerByStates();
	int ret = -1;
	//TODO
	ret = 0;
		 if (handler) {

				fprintf(stderr, "%s: %s[%d] handler->run_parsing_command()\n", __FILE__, __FUNCTION__, __LINE__);
				handler->run_parsing_command();
				sleep(1);

			fprintf(stderr, "%s: %s[%d] handler is deleted\n", __FILE__, __FUNCTION__, __LINE__);
			delete handler;
			handler = NULL;
		} else {
			fprintf(stderr, "%s: %s[%d] handler create is fail\n", __FILE__, __FUNCTION__, __LINE__);
			sleep(1);
		}

	return ret;
}

int Jobber::release()
{
	fprintf(stdout, "%s: %s[%d] Jobber is released\n", __FILE__, __FUNCTION__, __LINE__);
	int ret = -1;
	//TODO
	ret = 0;
	return ret;
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
		fprintf(stderr, "%s: %s[%d] jobber is new\n", __FILE__, __FUNCTION__, __LINE__);
		job = new Jobber();
		if (job != NULL) {
			if(!job->init()) {
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


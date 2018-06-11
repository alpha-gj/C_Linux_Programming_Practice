#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#define NUM_THREADS 4
#define check_error(return_val, msg) { \
	if (return_val != 0) { \
	fprintf(stderr, "%s: %s\n", msg, strerror(return_val)); \
	exit(-1); \
} \
}

#if 0
void *threadFunc(void* arg)
{
	/* pthread_detach(pthread_self()); */
	for (int i = 0; i < 100 ; i++)
	{
		sleep(1);
		printf("i is %d\n", i);
	}

	char *s = (char *) arg;
	printf("arg is %s\n", arg);
	return (void *) strlen(s);
}

int main(int argc , char *args[])
{
	pthread_t id;
	void *res;
	int ret = -1;
	char *str = "hello world";

	ret = pthread_create(&id, NULL, threadFunc, str);
	if (ret != 0)
		printf("pthread_create is error\n");

	/* if (pthread_equal(id, pthread_self())) */
	/* 		printf("tid matches self\n"); */

//	pthread_detach(id);
	printf("Message from main\n");
	sleep(3);
	/* ret = pthread_join(id, &res); */
	/* /1* ret = pthread_join(pthread_self(), NULL); *1/ */
	printf("Will cancel thread\n");
	sleep(3);
	pthread_cancel(id);
	printf("hello~, it is %d\n", (int *) res);
	pthread_exit(NULL);
	return 0;
}

#else

void Hello(void *t)
{
	pthread_t my_tid = pthread_self();
	printf("\t Thread %d: my tid is %lx, Hello World!\n", t, my_tid);
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	int rv, t;

	for (t = 0; t < NUM_THREADS; t++) {
		rv = pthread_create(&tid, NULL, (void *(*)(void *))Hello, (void *)t);
		check_error(rv, "pthread_create()");
		printf("Create thread %lx\n", tid);
	}

	printf("GoodBye\n");
	pthread_exit(NULL);
}

#endif

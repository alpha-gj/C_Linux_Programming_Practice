#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void clean_fun1(void * arg)
{
    printf("this is clean fun1\n");

    int s = 0;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        printf("pthread_mutex_unlock fail\n");
    }
}
void *thread_fun(void * arg)
{
    int s = 0;

    pthread_cleanup_push(clean_fun1,NULL);
    //pthread_cleanup_push(pthread_mutex_unlock, (void *) &mtx);

    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        printf("pthread_mutex_lock fail\n");
    }

    printf("hi this is the thread fun start\n");
    sleep(5); // cancel point, the pthread_cancel works here;
    //这里要注意,如果将sleep(5);换成while(1);的话,程序会一直暂停.push和pop要成对出现.
    //因为while(1) is not a cancel point, so 线程不接受cancel信号
    printf("hi this is the thread fun stop\n");

    printf("hi this is the thread fun finish\n");
    pthread_exit(0);

    pthread_cleanup_pop(0);
//  pthread_exit(0); // no put here afer pthread_cleanup_pop located beacasue the cleanup_pop can't work; (GJ)
}

int main()
{
    int err = 0;
    pthread_t tid1;
    pthread_t tid2;
    err=pthread_create(&tid1,NULL,thread_fun,NULL);
    err=pthread_create(&tid2,NULL,thread_fun,NULL);

    if(err != 0) {
        perror("pthread_create");
        exit(0);
    }

    printf("pthread cancel tid1\n");
    err = pthread_cancel(tid1);
    if(err != 0) {
        perror("cancel error:");
        exit(0);
    }

    printf("pthread join tid1\n");
    err = pthread_join(tid1,NULL);
    if(err != 0) {
        perror("pthread_join error:");
        exit(0);
    }

    printf("pthread join tid2\n");
    err = pthread_join(tid2,NULL);
    if(err != 0) {
        perror("pthread_join error:");
        exit(0);
    }

    return 0;
}

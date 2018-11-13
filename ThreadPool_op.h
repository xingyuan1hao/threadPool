#ifndef THREADPOOL_OP_H
#define THREADPOOL_OP_H
#include <pthread.h>

typedef void*( *Start_Work )( void *args );

typedef struct ThreadWorker
{
    Start_Work doWork;                        //thread work function
    void *args;                                                                            //thread work param
    struct ThreadWorker *next;

}ThreadWorker;

typedef struct ThreadPool
{
    int                                maxThreadNum;
    int                                curWorkLine;
    int                                shutDown;
    pthread_mutex_t         lock;
    pthread_cond_t            ready;
    ThreadWorker              *head;
    pthread_t                     *threadId;

}ThreadPool;

//      init threadpool
int ThreadPool_Init(int num);

void *Thread_Routine( void *args );

int ThreadPool_AddWork (Start_Work doWork, void *args);

int ThreadPool_Destroy();


#endif // THREADPOOL_OP_H

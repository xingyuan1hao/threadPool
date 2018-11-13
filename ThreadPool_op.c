#include "ThreadPool_op.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ThreadPool *pool = NULL;                                //  threadPool handle (whole situation)

int ThreadPool_Init( int num )
{
    int i;
    int ret = 0;
    //      init param
    pool = ( ThreadPool * ) malloc ( sizeof ( ThreadPool ) );                                                  //  create a thread pool
    memset ( pool, 0, sizeof (ThreadPool) );
    if( pool == NULL )
    {
        printf ( "pool Init error\n" );
        return -1;
    }

    pthread_mutex_init ( &( pool ->lock ), NULL );                                                                              //  initialize the mutex
    pthread_cond_init ( &( pool ->ready ), NULL );                                                                              //  initialize the cond
    pool ->maxThreadNum = num;
    pool ->curWorkLine = 0;
    pool ->shutDown = 0;                                                                                                                     //  not shutdown

    ret = (pool ->threadId) = ( pthread_t * ) malloc ( sizeof ( pthread_t ) * (pool ->maxThreadNum) );      //  get num pthreadId
    if( ret == NULL )
    {
        printf ( "pool ->threadId error\n" );
        return -1;
    }

    //      get "num" thread id;
    for( i = 0; i <= (pool ->maxThreadNum); ++i )
    {
        ret = pthread_create ( &(pool ->threadId[ i ]), NULL, Thread_Routine, NULL );
        if( ret != 0 )
        {
            return -1;
        }
    }

    return 0;
}

   //   This function (doWork) is public resource. the thread will excute when getting cpu.
void *Thread_Routine(void *args)
{
    //      thread excute success, it will exit;
    while(1)
    {
        printf ( " Thread %u create successful\n ", pthread_self () );
        pthread_mutex_lock ( &( pool ->lock ) );                                                       // lock
        while ( ((pool ->shutDown) == 0) && \
                     (pool ->curWorkLine) == 0 )                                                             // if thread not exit or have no work to do
        {
            printf ( " Thread %u is waiting for working\n ", pthread_self () );               // print whitch thread is ready
            pthread_cond_wait ( &( pool ->ready ), &( pool ->lock ) );
        }

        if( ( pool ->shutDown ) == 1 )                                                                       //  begin destroy
        {
            pthread_mutex_unlock ( &( pool ->lock ) );
            pthread_exit( NULL );
        }

        //  do work
        (pool ->curWorkLine) --;                                                                             //  waitting line decrease 1
        ThreadWorker *worker = (pool ->head);                                                    //  get a task
        pool ->head = (pool ->head) ->next;
        pthread_mutex_unlock ( &(pool ->lock) );                                                 //  unlock;
        (*( worker ->doWork )) ( worker ->args );                                                 //  do work

        free ( worker );                                                                                            //  free the worker link node
        worker = NULL;
    }

}

int ThreadPool_AddWork(Start_Work doWork, void *args)
{
    ThreadWorker *pCur = NULL;
    ThreadWorker *pTmp = ( ThreadWorker * ) malloc ( sizeof ( ThreadWorker ) );
    memset ( pTmp, 0, sizeof (ThreadWorker) );
    pTmp ->doWork = doWork;
    pTmp ->args = args;
    pTmp ->next = NULL;


    //  get last pool work link node
    pthread_mutex_lock ( &(pool ->lock) );

    pCur = pool ->head;
    while ( 1 )
    {
        if( (pool ->head) == NULL )
        {
            pool ->head = pTmp;
            break;
        }
        else if ( pCur ->next == NULL )
        {
            pCur ->next = pTmp;
            break;
        }

        pCur = pCur ->next;
    }

    ( pool ->curWorkLine ) ++;

    pthread_mutex_unlock ( &(pool ->lock) );
    pthread_cond_signal ( &(pool ->ready) );

    return 0;
}

int ThreadPool_Destroy()
{
       int i;
    //
    if( (pool ->shutDown) == 1 )                                //  already destroy
    {
        return -1;
    }

    pool ->shutDown = 1;
    pthread_cond_broadcast ( &(pool ->ready) );     //   All thread will "excute pthread_exit()";

    //      join
    for( i = 0; i < (pool ->maxThreadNum); ++i )
    {
        printf ( "The thread: %u is exiting\n", pthread_self () );
        pthread_join ( (pool ->threadId[ i ]), NULL );
    }

    if( (pool ->threadId) != NULL )
    {
        free ( pool ->threadId );
    }

    //     destroy  task line;
    ThreadWorker *pHead = NULL;
    ThreadWorker *pNext = NULL;
    pHead = (pool ->head);
    while( pHead != NULL )
    {
        pNext = pHead ->next;
        free( pHead );
        pHead = pNext;
        pHead = pHead ->next;
    }

    //      destroy mutex
    pthread_mutex_destroy ( &(pool ->lock) );
    pthread_cond_destroy ( &(pool ->ready) );

    if( pool != NULL )
    {
        free ( pool );
    }

    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include "ThreadPool_op.h"

static int count = 0;
pthread_mutex_t myTest;

void* My_Work( void *args );

int main(void)
{
    int ret = 0;
    int num = 20;                                                                   // initialize 20 threads;
#if 1
    pthread_mutex_init ( &myTest, NULL );

    ret = ThreadPool_Init ( num );
    if( ret < 0 )
    {
        printf( "Thread init error \n" );
        ThreadPool_Destroy ();
    }

    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );
    ThreadPool_AddWork( My_Work, NULL );

    sleep( 10000 );
    ThreadPool_Destroy ();
    pthread_mutex_destroy ( &(myTest) );
#endif
    return 0;
}

void* My_Work( void *args )
{
    pthread_mutex_lock( &myTest );
    printf( " The thread %u \tcount: %d \n", pthread_self (), ++count );
    pthread_mutex_unlock ( &myTest );
}


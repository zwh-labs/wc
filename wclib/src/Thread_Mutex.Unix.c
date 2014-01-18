#include "Thread_Mutex.h"

#include <pthread.h>

#include <stdbool.h>
#include <stdlib.h>


struct _wcThread_Mutex
{
	pthread_mutex_t pthreadMutex;
};


wcThread_Mutex * wcThread_Mutex_new()
{
	wcThread_Mutex * mutex = (wcThread_Mutex*) malloc( sizeof(wcThread_Mutex) );
	if( !mutex )
		return NULL;

	if( pthread_mutex_init( &(mutex->pthreadMutex), NULL ) != 0 )
	{
		free( mutex );
		return NULL;
	}

	return mutex;
}


bool wcThread_Mutex_lock( wcThread_Mutex * mutex )
{
	return pthread_mutex_lock( &(mutex->pthreadMutex) ) == 0;
}


bool wcThread_Mutex_unlock( wcThread_Mutex * mutex )
{
	return pthread_mutex_unlock( &(mutex->pthreadMutex) ) == 0;
}


bool wcThread_Mutex_delete( wcThread_Mutex * mutex )
{
	if( !mutex )
		return false;
	bool ret = pthread_mutex_destroy( &(mutex->pthreadMutex) ) == 0;
	free( mutex );
	return ret;
}

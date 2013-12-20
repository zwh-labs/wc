#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct WCThread_Mutex
{
	pthread_mutex_t pthreadMutex;
} WCThread_Mutex;


WCThread_Mutex * WCThread_Mutex_new()
{
	WCThread_Mutex * mutex = malloc( sizeof(WCThread_Mutex) );
	if( !mutex )
		return NULL;
	if( pthread_mutex_init( &(mutex->pthreadMutex), NULL ) != 0 )
	{
		free( mutex );
		return NULL;
	}
	return mutex;
}


bool WCThread_Mutex_lock( WCThread_Mutex * mutex )
{
	return pthread_mutex_lock( &(mutex->pthreadMutex) ) == 0;
}


bool WCThread_Mutex_unlock( WCThread_Mutex * mutex )
{
	return pthread_mutex_unlock( &(mutex->pthreadMutex) ) == 0;
}


bool WCThread_Mutex_delete( WCThread_Mutex * mutex )
{
	bool ret = pthread_mutex_destroy( &(mutex->pthreadMutex) ) == 0;
	free( mutex );
	return ret;
}

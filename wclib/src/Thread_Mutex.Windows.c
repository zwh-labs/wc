#include "Thread_Mutex.h"

#include <windows.h>

#include <stdbool.h>
#include <stdlib.h>


struct _wcThread_Mutex
{
	HANDLE winMutex;
};


wcThread_Mutex * wcThread_Mutex_new()
{
	wcThread_Mutex * mutex = (wcThread_Mutex*) malloc( sizeof(wcThread_Mutex) );
	if( !mutex )
		return NULL;

	mutex->winMutex = CreateMutex( NULL, FALSE, NULL );
	if( !mutex->winMutex )
	{
		free( mutex );
		return NULL;
	}

	return mutex;
}


bool wcThread_Mutex_lock( wcThread_Mutex * mutex )
{
	return WaitForSingleObject( mutex->winMutex, INFINITE ) == WAIT_OBJECT_0;
}


bool wcThread_Mutex_unlock( wcThread_Mutex * mutex )
{
	return ReleaseMutex( mutex->winMutex );
}


bool wcThread_Mutex_delete( wcThread_Mutex * mutex )
{
	if( !mutex )
		return false;
	bool ret = CloseHandle( &(mutex->winMutex) );
	free( mutex );
	return ret;
}

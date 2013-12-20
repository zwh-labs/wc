#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct WCThread_Mutex
{
	HANDLE winMutex;
} WCThread_Mutex;


WCThread_Mutex * WCThread_Mutex_new()
{
	WCThread_Mutex * mutex = malloc( sizeof(WCThread_Mutex) );
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


bool WCThread_Mutex_lock( WCThread_Mutex * mutex )
{
	return WaitForSingleObject( mutex->winMutex, INFINITE ) == WAIT_OBJECT_0;
}


bool WCThread_Mutex_unlock( WCThread_Mutex * mutex )
{
	return ReleaseMutex( mutex->winMutex );
}


bool WCThread_Mutex_delete( WCThread_Mutex * mutex )
{
	bool ret = CloseHandle( &(mutex->winMutex) );
	free( mutex );
	return ret;
}

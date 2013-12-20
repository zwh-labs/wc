#include "WCThread_ThreadMain.h"

#include <windows.h>

#include <stdbool.h>
#include <stdlib.h>


typedef struct WCThread_Thread
{
	HANDLE winThread;
	DWORD id;
	WCThread_ThreadMain threadMain;
	void * threadData;
} WCThread_Thread;


static DWORD WINAPI starter( LPVOID threadData )
{
	WCThread_Thread * thread = (WCThread_Thread*) threadData;
	thread->threadMain( thread->threadData );
	return 0;
}


WCThread_Thread * WCThread_Thread_new( WCThread_ThreadMain threadMain, void * data )
{
	WCThread_Thread * thread = malloc( sizeof(WCThread_Thread) );
	if( !thread )
		return NULL;
	thread->threadData = data;
	thread->threadMain = threadMain;

	thread->winThread = CreateThread(
		NULL,       // default security attributes
		0,          // default stack size
		starter,
		thread,
		0,          // default creation flags
		&(thread->id) ); // receive thread identifier

	if( !thread->winThread )
	{
		free( thread );
		return NULL;
	}
	return thread;
}


bool WCThread_Thread_join( WCThread_Thread * thread )
{
	return WaitForSingleObject( thread->winThread, INFINITE );
}


bool WCThread_Thread_delete( WCThread_Thread * thread )
{
	bool ret = CloseHandle( thread->winThread );
	free( thread );
	return ret;
}

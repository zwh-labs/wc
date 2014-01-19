#include "Thread_Thread.h"
#include "Thread_ThreadMain.h"

#include <windows.h>

#include <stdbool.h>
#include <stdlib.h>


struct _wcThread_Thread
{
	HANDLE winThread;
	DWORD id;
	wcThread_ThreadMain threadMain;
	void * threadData;
};


static DWORD WINAPI starter( LPVOID threadData )
{
	wcThread_Thread * thread = (wcThread_Thread*) threadData;
	thread->threadMain( thread->threadData );
	return 0;
}


wcThread_Thread * wcThread_Thread_new( wcThread_ThreadMain threadMain, void * data )
{
	wcThread_Thread * thread = (wcThread_Thread*) malloc( sizeof(wcThread_Thread) );
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


bool wcThread_Thread_join( wcThread_Thread * thread )
{
	return WaitForSingleObject( thread->winThread, INFINITE ) == WAIT_OBJECT_0;
}


bool wcThread_Thread_delete( wcThread_Thread * thread )
{
	bool ret = CloseHandle( thread->winThread );
	free( thread );
	return ret;
}

#include "WCThread_Thread.h"
#include "WCThread_ThreadMain.h"

#include <pthread.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


struct _WCThread_Thread
{
	pthread_t pthreadThread;
	WCThread_ThreadMain threadMain;
	void * threadData;
};


static void * starter( void * threadData )
{
	WCThread_Thread * thread = (WCThread_Thread*) threadData;
//	fprintf( stderr, "Executing %p with %p\n", (void*)thread->threadMain, thread->threadData );
	thread->threadMain( thread->threadData );
	return NULL;
}


WCThread_Thread * WCThread_Thread_new( WCThread_ThreadMain threadMain, void * data )
{
	WCThread_Thread * thread = malloc( sizeof(WCThread_Thread) );
	if( !thread )
		return NULL;
	thread->threadData = data;
	thread->threadMain = threadMain;
	if( pthread_create( &(thread->pthreadThread), NULL, starter, thread ) != 0 )
	{
		free( thread );
		return NULL;
	}
	return thread;
}


bool WCThread_Thread_join( WCThread_Thread * thread )
{
	return pthread_join( thread->pthreadThread, NULL ) == 0;
}


bool WCThread_Thread_delete( WCThread_Thread * thread )
{
	free( thread );
	return true;
}

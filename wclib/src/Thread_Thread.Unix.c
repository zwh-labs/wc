#include "Thread_Thread.h"
#include "Thread_ThreadMain.h"

#include <pthread.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


struct _wcThread_Thread
{
	pthread_t pthreadThread;
	wcThread_ThreadMain threadMain;
	void * threadData;
};


static void * starter( void * threadData )
{
	wcThread_Thread * thread = (wcThread_Thread*) threadData;
//	fprintf( stderr, "Executing %p with %p\n", (void*)thread->threadMain, thread->threadData );
	thread->threadMain( thread->threadData );
	return NULL;
}


wcThread_Thread * wcThread_Thread_new( wcThread_ThreadMain threadMain, void * data )
{
	wcThread_Thread * thread = (wcThread_Thread*) malloc( sizeof(wcThread_Thread) );
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


bool wcThread_Thread_join( wcThread_Thread * thread )
{
	return pthread_join( thread->pthreadThread, NULL ) == 0;
}


bool wcThread_Thread_delete( wcThread_Thread * thread )
{
	free( thread );
	return true;
}

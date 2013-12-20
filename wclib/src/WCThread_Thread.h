#ifndef _WCTHREAD_THREAD_H_
#define _WCTHREAD_THREAD_H_


#include "WCThread_ThreadMain.h"

#include <stdbool.h>


typedef struct WCThread_Thread WCThread_Thread;


WCThread_Thread * WCThread_Thread_new( WCThread_ThreadMain threadMain, void * data );
bool WCThread_Thread_join( WCThread_Thread * thread );
bool WCThread_Thread_delete( WCThread_Thread * thread );


#endif

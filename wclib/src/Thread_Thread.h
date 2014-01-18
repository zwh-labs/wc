#ifndef _WC_THREAD_THREAD_H_
#define _WC_THREAD_THREAD_H_


#include "Thread_ThreadMain.h"

#include <stdbool.h>


typedef struct _wcThread_Thread wcThread_Thread;


wcThread_Thread * wcThread_Thread_new( wcThread_ThreadMain threadMain, void * data );
bool wcThread_Thread_join( wcThread_Thread * thread );
bool wcThread_Thread_delete( wcThread_Thread * thread );


#endif

#ifndef _WCTHREAD_MUTEX_H_
#define _WCTHREAD_MUTEX_H_


#include <stdbool.h>


typedef struct _wcThread_Mutex wcThread_Mutex;


wcThread_Mutex * wcThread_Mutex_new();
bool wcThread_Mutex_lock( wcThread_Mutex * mutex );
bool wcThread_Mutex_unlock( wcThread_Mutex * mutex );
bool wcThread_Mutex_delete( wcThread_Mutex * mutex );


#endif

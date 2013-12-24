#ifndef _WCTHREAD_MUTEX_H_
#define _WCTHREAD_MUTEX_H_


#include <stdbool.h>


typedef struct _WCThread_Mutex WCThread_Mutex;


WCThread_Mutex * WCThread_Mutex_new();
bool WCThread_Mutex_lock( WCThread_Mutex * mutex );
bool WCThread_Mutex_unlock( WCThread_Mutex * mutex );
bool WCThread_Mutex_delete( WCThread_Mutex * mutex );


#endif

#ifndef _WCTHREAD_H_
#define _WCTHREAD_H_


#include <wc/WCConnection.h>


typedef struct WCThread WCThread;


WCThread * WCThread_start( WCConnection * connection );
int WCThread_stop( WCThread * thread );
int WCThread_retrieveWheel( WCThread * thread, int channel );
int WCThread_getWheelCount( WCThread * thread );


#endif

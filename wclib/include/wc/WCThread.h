#ifndef _WCTHREAD_H_
#define _WCTHREAD_H_


#include <wc/WCConnection.h>
#include <wc/WCWheelMovement.h>


typedef struct WCThread WCThread;


WCThread * WCThread_start( WCConnection * connection );
int WCThread_stop( WCThread * thread );
WCWheelMovement WCThread_retrieveWheelMovement( WCThread * thread, unsigned int index );
int WCThread_getWheelCount( WCThread * thread );


#endif

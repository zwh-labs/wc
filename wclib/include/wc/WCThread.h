#ifndef _WCTHREAD_H_
#define _WCTHREAD_H_


#include <wc/WCConnection.h>
#include <wc/WCWheelMovement.h>

#include <stdbool.h>


typedef struct _WCThread WCThread;


WCThread * WCThread_start( WCConnection * connection );
bool WCThread_stop( WCThread * thread );

WCWheelMovement WCThread_retrieveWheelMovement( WCThread * thread, unsigned int index );
unsigned int WCThread_getWheelCount( WCThread * thread );


#endif

#ifndef _WC_THREAD_H_
#define _WC_THREAD_H_


#include <wc/Connection.h>
#include <wc/WheelMovement.h>

#include <stdbool.h>


typedef struct _wcThread wcThread;


wcThread * wcThread_start( wcConnection * connection );
bool wcThread_stop( wcThread * thread );

wcWheelMovement wcThread_retrieveWheelMovement( wcThread * thread, unsigned int index );
unsigned int wcThread_getWheelCount( wcThread * thread );


#endif

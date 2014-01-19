#ifndef _WC_THREAD_H_
#define _WC_THREAD_H_


#include "wc_export.h"
#include <wc/Connection.h>
#include <wc/WheelMovement.h>

#include <stdbool.h>


typedef struct _wcThread wcThread;


#ifdef __cplusplus
extern "C" {
#endif

WC_EXPORT wcThread * wcThread_start( wcConnection * connection );
WC_EXPORT bool wcThread_stop( wcThread * thread );

WC_EXPORT wcWheelMovement wcThread_retrieveWheelMovement( wcThread * thread, unsigned int index );
WC_EXPORT unsigned int wcThread_getWheelCount( wcThread * thread );

#ifdef __cplusplus
}
#endif


#endif

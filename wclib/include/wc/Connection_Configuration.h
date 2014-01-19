#ifndef _WC_CONNECTION_CONFIGURATION_H_
#define _WC_CONNECTION_CONFIGURATION_H_


#include "wc_export.h"
#include <wc/Connection.h>
#include <wc/Configuration.h>


#ifdef __cplusplus
extern "C" {
#endif

WC_EXPORT wcConnection * wcConnection_openFromConfiguration( const wcConfiguration * configuration );

#ifdef __cplusplus
}
#endif


#endif

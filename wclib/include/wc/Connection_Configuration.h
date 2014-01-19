#ifndef _WC_CONNECTION_CONFIGURATION_H_
#define _WC_CONNECTION_CONFIGURATION_H_


#include "wc_export.h"
#include <wc/Connection.h>
#include <wc/Configuration.h>


WC_EXPORT wcConnection * wcConnection_openFromConfiguration( const wcConfiguration * configuration );


#endif

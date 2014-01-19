#ifndef _WC_CONFIGURATION_STDIO_H_
#define _WC_CONFIGURATION_STDIO_H_


#include "wc_export.h"
#include <wc/Configuration.h>

#include <stdio.h>


WC_EXPORT int wcConfiguration_snprint( char * s, size_t n, const wcConfiguration * configuration );
WC_EXPORT int wcConfiguration_fprint( FILE * stream, const wcConfiguration * configuration );
WC_EXPORT int wcConfiguration_print( const wcConfiguration * configuration );


#endif

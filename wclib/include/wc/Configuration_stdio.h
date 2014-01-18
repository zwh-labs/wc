#ifndef _WC_CONFIGURATION_STDIO_H_
#define _WC_CONFIGURATION_STDIO_H_


#include <wc/Configuration.h>

#include <stdio.h>


int wcConfiguration_snprint( char * s, size_t n, const wcConfiguration * configuration );
int wcConfiguration_fprint( FILE * stream, const wcConfiguration * configuration );
int wcConfiguration_print( const wcConfiguration * configuration );


#endif

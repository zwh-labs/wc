#ifndef _WC_CONFIGURATION_H_
#define _WC_CONFIGURATION_H_


#include "wc_export.h"
#include <stdbool.h>


typedef struct _wcConfiguration wcConfiguration;


WC_EXPORT const char * wcConfiguration_getDevicePath( const wcConfiguration * configuration );
WC_EXPORT unsigned int wcConfiguration_getWheelCount( const wcConfiguration * configuration );
WC_EXPORT unsigned int wcConfiguration_getWheelIncrementsPerTurn( const wcConfiguration * configuration, unsigned int wheelIndex );

WC_EXPORT void wcConfiguration_setDevicePath( wcConfiguration * configuration, const char * devicePath );
WC_EXPORT bool wcConfiguration_setWheel( wcConfiguration * configuration, unsigned int wheelIndex, unsigned int incrementsPerTurn );

WC_EXPORT bool wcConfiguration_delete( wcConfiguration * configuration );
WC_EXPORT wcConfiguration * wcConfiguration_new();


#endif

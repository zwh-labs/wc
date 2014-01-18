#ifndef _WC_CONFIGURATION_H_
#define _WC_CONFIGURATION_H_


#include <stdbool.h>


typedef struct _wcConfiguration wcConfiguration;


const char * wcConfiguration_getDevicePath( const wcConfiguration * configuration );
unsigned int wcConfiguration_getWheelCount( const wcConfiguration * configuration );
unsigned int wcConfiguration_getWheelIncrementsPerTurn( const wcConfiguration * configuration, unsigned int wheelIndex );

void wcConfiguration_setDevicePath( wcConfiguration * configuration, const char * devicePath );
bool wcConfiguration_setWheel( wcConfiguration * configuration, unsigned int wheelIndex, unsigned int incrementsPerTurn );

bool wcConfiguration_delete( wcConfiguration * configuration );
wcConfiguration * wcConfiguration_new();


#endif

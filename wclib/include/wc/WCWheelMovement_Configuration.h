#ifndef _WCWHEELMOVEMENT_CONFIGURATION_H_
#define _WCWHEELMOVEMENT_CONFIGURATION_H_


#include <wc/WCWheelMovement.h>
#include <wc/WCConfiguration.h>


double WCWheelMovement_getIncrementsAsTurns( const WCWheelMovement * wm, const WCConfiguration * c );
double WCWheelMovement_getIncrementsAsDistance( const WCWheelMovement * wm, const WCConfiguration * c );


#endif

#include <wc/WCWheelMovement.h>
#include <wc/WCConfiguration.h>


#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif


double WCWheelMovement_getIncrementsAsTurns( const WCWheelMovement * wm, const WCConfiguration * c )
{
	return (double)WCWheelMovement_getIncrements( wm )
		/ (double)WCConfiguration_getWheelIncrementsPerTurn( c, WCWheelMovement_getIndex( wm ) );
}

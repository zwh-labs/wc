#include <wc/WheelMovement.h>
#include <wc/Configuration.h>


#ifndef M_PI
#define M_PI (3.1415926535897932384626433832795)
#endif


double wcWheelMovement_getTurns( const wcWheelMovement * wm, const wcConfiguration * c )
{
	return (double)wcWheelMovement_getIncrements( wm )
		/ (double)wcConfiguration_getWheelIncrementsPerTurn( c, wcWheelMovement_getIndex( wm ) );
}

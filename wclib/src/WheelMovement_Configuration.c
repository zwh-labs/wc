#include <wc/WheelMovement_Configuration.h>
#include <wc/Configuration.h>


double wcWheelMovement_getTurns( const wcWheelMovement * wm, const wcConfiguration * c )
{
	return (double)wcWheelMovement_getIncrements( wm )
		/ (double)wcConfiguration_getWheelIncrementsPerTurn( c, wcWheelMovement_getIndex( wm ) );
}

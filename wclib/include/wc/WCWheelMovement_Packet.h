#ifndef _WCWHEELMOVEMENT_PACKET_H_
#define _WCWHEELMOVEMENT_PACKET_H_


#include <wc/WCWheelMovement.h>
#include <wc/WCPacket.h>


static inline void WCWheelMovement_createFromPacket( WCWheelMovement * wm, const WCPacket_Wheel * pw )
{
	WCWheelMovement_create( wm, pw->channel );
	WCWheelMovement_accumulate( wm, pw->value, pw->error );
}


static inline void WCWheelMovement_accumulateFromPacket( WCWheelMovement * wm, const WCPacket_Wheel * pw )
{
	WCWheelMovement_accumulate( wm, pw->value, pw->error );
}


#endif

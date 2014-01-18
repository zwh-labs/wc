#ifndef _WC_WHEELMOVEMENT_PACKET_H_
#define _WC_WHEELMOVEMENT_PACKET_H_


#include <wc/WheelMovement.h>
#include <wc/Packet.h>


static inline void wcWheelMovement_createFromPacket( wcWheelMovement * wm, const wcPacket_Wheel * pw )
{
	wcWheelMovement_create( wm, pw->channel );
	wcWheelMovement_accumulate( wm, pw->value, pw->error );
}


static inline void wcWheelMovement_accumulateFromPacket( wcWheelMovement * wm, const wcPacket_Wheel * pw )
{
	wcWheelMovement_accumulate( wm, pw->value, pw->error );
}


#endif

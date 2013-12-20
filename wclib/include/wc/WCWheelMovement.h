#ifndef _WCWHEELMOVEMENT_H_
#define _WCWHEELMOVEMENT_H_


#include <wc/WCError.h>


typedef struct WCWheelMovement
{
	unsigned int index;
	int increments;
	WCError error;
} WCWheelMovement;


static inline unsigned int WCWheelMovement_getIndex( const WCWheelMovement * wm )
	{ return wm->index; }

static inline int WCWheelMovement_getIncrements( const WCWheelMovement * wm )
	{ return wm->increments; }

static inline WCError WCWheelMovement_getError( const WCWheelMovement * wm )
	{ return wm->error; }


static inline void WCWheelMovement_create( WCWheelMovement * wm, unsigned int index )
	{ wm->index = index; wm->increments = 0; wm->error = WCERROR_NOERROR; }

static inline void WCWheelMovement_reset( WCWheelMovement * wm )
	{ wm->increments = 0; wm->error = WCERROR_NOERROR; }

static inline void WCWheelMovement_accumulate( WCWheelMovement * wm, int increment, WCError error )
	{ wm->increments += increment; wm->error |= error; }


#endif

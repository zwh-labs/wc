#ifndef _WC_WHEELMOVEMENT_H_
#define _WC_WHEELMOVEMENT_H_


#include <wc/Error.h>


typedef struct
{
	unsigned int index;
	int increments;
	wcError error;
} wcWheelMovement;


#ifdef _MSC_VER // C99 anyone?
	#define inline __forceinline
#endif


static inline unsigned int wcWheelMovement_getIndex( const wcWheelMovement * wm )
	{ return wm->index; }

static inline int wcWheelMovement_getIncrements( const wcWheelMovement * wm )
	{ return wm->increments; }

static inline wcError wcWheelMovement_getError( const wcWheelMovement * wm )
	{ return wm->error; }


static inline void wcWheelMovement_create( wcWheelMovement * wm, unsigned int index )
	{ wm->index = index; wm->increments = 0; wm->error = WC_ERROR_NOERROR; }

static inline void wcWheelMovement_reset( wcWheelMovement * wm )
	{ wm->increments = 0; wm->error = WC_ERROR_NOERROR; }

static inline void wcWheelMovement_accumulate( wcWheelMovement * wm, int increment, wcError error )
	{ wm->increments += increment; wm->error |= error; }


#endif

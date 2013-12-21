#include "IncRotDec.h"
#include <util/atomic.h>


// returns the decoded position from encoder
static inline uint8_t readPosition( IncRotDec * incRotDec )
{
	uint8_t position = 0;
	uint8_t input = *(incRotDec->pinRegister);
	if( input & (incRotDec->pinMaskA) )
		position = 3;
	if( input & (incRotDec->pinMaskB) )
		position ^= 1;
	return position;
}


void IncRotDec_init( IncRotDec * incRotDec, volatile uint8_t * dataDirectionRegister, volatile uint8_t * pinRegister, volatile uint8_t * portRegister, uint8_t pinMaskA, uint8_t pinMaskB )
{
	*dataDirectionRegister &= ~( pinMaskA | pinMaskB ); // set pins as inputs
	*portRegister &= ~( pinMaskA | pinMaskB );          // disable pullups

	incRotDec->dataDirectionRegister = dataDirectionRegister;
	incRotDec->pinRegister = pinRegister;
	incRotDec->portRegister = portRegister;
	incRotDec->pinMaskA = pinMaskA;
	incRotDec->pinMaskB = pinMaskB;
	incRotDec->lastError = INCROTDEC_ERROR_NOERROR;
	incRotDec->value = 0;

	// read current position from hardware as initial state
	incRotDec->lastPosition = readPosition( incRotDec );
}


void IncRotDec_sample( IncRotDec * incRotDec )
{
	uint8_t position = readPosition( incRotDec );
	int8_t diff = incRotDec->lastPosition - position;
	if( diff & 1 ) // bit 0 not set if no movement or skipped a state
		incRotDec->value += (diff & 2) - 1; // bit 1 set on increment
	else if( diff & 2 )
		incRotDec->lastError = INCROTDEC_ERROR_DECODING;
	incRotDec->lastPosition = position;
}


uint8_t IncRotDec_retrieve( IncRotDec * incRotDec, int16_t * value )
{
	uint8_t error;
	ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
	{
		*value = incRotDec->value;
		incRotDec->value = 0;
		error = incRotDec->lastError;
		incRotDec->lastError = INCROTDEC_ERROR_NOERROR;
	}
	return error;
}

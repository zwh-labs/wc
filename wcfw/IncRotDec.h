#ifndef _INCROTDEC_H_
#define _INCROTDEC_H_

#include <stdint.h>


/// No error detected
#define INCROTDEC_ERROR_NOERROR  0
/// Used to signify a decoder error (movement too fast or signals unclean)
#define INCROTDEC_ERROR_DECODING 1


/// Incremental rotary encoder structure for each available encoder
typedef struct
{
	volatile uint8_t * dataDirectionRegister; ///< The data direction register address the encoder is connected to
	volatile uint8_t * pinRegister;           ///< The pin register address the encoder is connected to
	volatile uint8_t * portRegister;          ///< The port register address the encoder is connected to
	uint8_t pinMaskA;         ///< Bitmask for input A
	uint8_t pinMaskB;         ///< Bitmask for input B
	uint8_t lastPosition;     ///< Last encoder position - only to be used in IncRotDec_sample(IncRotDec*)
	volatile int8_t lastError; ///< Last error detected
	volatile int16_t value;   ///< Current value
} IncRotDec;


/// Initialises a new incremental rotary encoder
void IncRotDec_init( IncRotDec * incRotDec, volatile uint8_t * dataDirectionRegister, volatile uint8_t * pinRegister, volatile uint8_t * portRegister, uint8_t pinMaskA, uint8_t pinMaskB );

/// Samples input pins
/**
 * Polls the input pins of given decoder.
 * This function may be used inside an interrupt.
 * @param incRotDec The decoder to sample - Must not be NULL.
 */
void IncRotDec_sample( IncRotDec * incRotDec );

/// Retrieves the number of increments/decrements since last retrieve call
/**
 * @param incRotDec The decoder to read from - Must not be NULL.
 * @param value Returns the current number of increments/decrements for given decoder - Must not be NULL.
 * @returns One of: INCROTDEC_ERROR_NOERROR, INCROTDEC_ERROR_DECODING
 */
uint8_t IncRotDec_retrieve( IncRotDec * incRotDec, int16_t * value );


#endif

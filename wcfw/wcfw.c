/** \file
 *
 *  Main source file for wcfw.
 */

#include "wcfw.h"
#include "IncRotDec.h"
#include "USBDescriptors.h"

#include <wc/WCPacket.h>

#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/USB/USB.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>


static IncRotDec incRotDec;
static WCPacket_Message message;


ISR( TIMER1_COMPA_vect )
{
	IncRotDec_sample( &incRotDec );
}


// timer for IncRotDec_sample
void timer1_init(void)
{
	// normal port operation OCnA/OCnB/OCnC disconnected; CTC mode
	TCCR1A = 0;

	//       CTC mode       clk/8
	TCCR1B = _BV(WGM12) | _BV(CS11);

	// not forcing output compare
	TCCR1C = 0;

	// timer counter initial value (16 bit value)
	TCNT1 = 0;

	// compare value
	OCR1A = 20;

	// enable Timer/Counter1 Output Compare A Match interrupt
	TIMSK1 = _BV(OCIE1A);
}

/*
ISR( TIMER3_COMPA_vect )
{
	WCPacket_Message_create( &message, "WCFW created on "__DATE__" "__TIME__ );
}


void timer3_init(void)
{
	// normal port operation OCnA/OCnB/OCnC disconnected; CTC mode
	TCCR3A = 0;

	//       CTC mode       clk/64
	TCCR3B = _BV(WGM32) | _BV(CS32) | _BV(CS30);

	// not forcing output compare
	TCCR3C = 0;

	// timer counter initial value (16 bit value)
	TCNT3 = 0;

	// compare value
	OCR3A = 20000;

	// enable Timer/Counter3 Output Compare A Match interrupt
	TIMSK3 = _BV(OCIE3A);
}
*/

void SetupHardware(void)
{
	// disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// disable clock division
	clock_prescale_set( clock_div_1 );

	LEDs_Init();
	USB_Init();

	IncRotDec_init( &incRotDec, &DDRF, &PINF, &PORTF, _BV(PINF0), _BV(PINF1) );
	timer1_init();
//	timer3_init();
}


int main(void)
{
	SetupHardware();
	GlobalInterruptEnable();
	while( 1 )
	{
		CDC_Task();
		USB_USBTask();
		_delay_ms(10);
	}
}


/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
	if( USB_DeviceState != DEVICE_STATE_Configured )
		return;

	Endpoint_SelectEndpoint( CDC_TX_EPADDR );

	if( message.header.length )
	{
		Endpoint_Write_Stream_LE( &message, WCPacket_size((WCPacket_Header*)&message), NULL );
		message.header.length = 0;
	}

	int16_t value = 0;
	uint8_t error = IncRotDec_retrieve( &incRotDec, &value );
	WCPacket_Wheel wheel;
	WCPacket_Wheel_create( &wheel, 0, error, value );
	Endpoint_Write_Stream_LE( &wheel, WCPacket_size((WCPacket_Header*)&wheel), NULL );

	bool isFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
	Endpoint_ClearIN();
	if( isFull )
	{
		Endpoint_WaitUntilReady();
		Endpoint_ClearIN();
	}

	Endpoint_SelectEndpoint( CDC_RX_EPADDR );
	if( Endpoint_IsOUTReceived() )
	{
		Endpoint_ClearOUT();
	}
}

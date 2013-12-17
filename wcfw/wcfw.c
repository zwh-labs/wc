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


void interpretPacket( const WCPacket * packet )
{
	switch( packet->header.type )
	{
		case WCPACKET_REQUESTINFO_TYPE:
			WCPacket_Message_create( &message, "#WCFW created on "__DATE__" "__TIME__ );
			break;
	}
}


/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
	if( USB_DeviceState != DEVICE_STATE_Configured )
		return;

	////////////////////////////////
	// write to host
	Endpoint_SelectEndpoint( CDC_TX_EPADDR );

	// write message if available
	if( message.header.length )
	{
		Endpoint_Write_Stream_LE( &message, WCPacket_size((const WCPacket*)&message), NULL );
		message.header.length = 0; // empty message, so we won't send the same message again
	}

	// write wheel counters
	int16_t value = 0;
	uint8_t error = IncRotDec_retrieve( &incRotDec, &value );
	WCPacket_Wheel wheel;
	WCPacket_Wheel_create( &wheel, 0, error, value );
	Endpoint_Write_Stream_LE( &wheel, WCPacket_size((const WCPacket*)&wheel), NULL );

	// clear
	bool isFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
	Endpoint_ClearIN();
	if( isFull )
	{
		Endpoint_WaitUntilReady();
		Endpoint_ClearIN();
	}

	////////////////////////////////
	// read from host
	Endpoint_SelectEndpoint( CDC_RX_EPADDR );
	if( Endpoint_IsOUTReceived() )
	{
		uint8_t ret;
		uint8_t buffer[WCPACKET_MAXSIZE];
		ret = Endpoint_Read_Stream_LE( buffer, sizeof(WCPacket_Header), NULL );
		if( ret == ENDPOINT_RWSTREAM_NoError )
		{
			WCPacket * packet = (WCPacket*)buffer;
			if( (sizeof(WCPacket_Header) + packet->header.length) > WCPACKET_MAXSIZE )
			{
				Endpoint_Discard_Stream( packet->header.length, NULL );
			}
			else
			{
				ret = Endpoint_Read_Stream_LE( buffer + sizeof(WCPacket_Header), packet->header.length, NULL );
				if( ret == ENDPOINT_RWSTREAM_NoError )
					interpretPacket( (const WCPacket*)buffer );
			}
		}
		Endpoint_ClearOUT();
	}
}

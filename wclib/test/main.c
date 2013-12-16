#include <wc/WCPacket.h>
#include <wc/WCIO_Unix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>


int main( void )
{
	int wcDevice;
	wcDevice = WCIO_Unix_open( "/dev/ttyACM0" );
	if( !wcDevice )
	{
		fprintf( stderr, "Cannot open connection\n" );
		exit( 1 );
	}

	uint8_t buffer[WCPACKET_MAXSIZE];
	WCPacket_RequestInfo_create( (WCPacket_RequestInfo*)buffer );
	WCIO_Unix_write( wcDevice, buffer );
	while( 1 )
	{
		WCPacket_Header * header = (WCPacket_Header*)buffer;
		WCIO_Unix_read( wcDevice, buffer );
		switch( header->type )
		{
			case WCPACKET_MESSAGE_TYPE:
			{
				WCPacket_Message * message = (WCPacket_Message*)buffer;
				message->text[message->header.length] = 0;
				fprintf( stderr, "Received message:\"%s\"\n", message->text );
				break;
			}
			case WCPACKET_WHEEL_TYPE:
			{
				WCPacket_Wheel * wheel = (WCPacket_Wheel*)buffer;
				fprintf( stderr, "Received wheel:\tchannel=%d\terror=%d\tvalue=%d\n", wheel->channel, wheel->error, wheel->value );
				break;
			}
			default:
			{
				fprintf( stderr, "Received unknown packet type: %d\n", header->type );
				break;
			}
		}
	}

	WCIO_Unix_close( wcDevice );
	return 0;
}

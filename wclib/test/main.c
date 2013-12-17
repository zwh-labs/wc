#include <wc/WCPacket.h>
#include <wc/WCConnection.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>


int main( void )
{
	WCConnection * connection;
#ifdef _WIN32
	connection = WCConnection_open( "\\\\.\\COM1" );
#else
	connection = WCConnection_open( "/dev/ttyACM0" );
#endif
	if( connection == NULL )
	{
		fprintf( stderr, "Cannot open connection\n" );
		exit( 1 );
	}

	uint8_t buffer[WCPACKET_MAXSIZE];
	WCPacket_RequestInfo_create( (WCPacket_RequestInfo*)buffer );
	WCConnection_write( connection, (const WCPacket*)buffer );
	while( 1 )
	{
		WCPacket_Header * header = (WCPacket_Header*)buffer;
		WCConnection_read( connection, (WCPacket*)buffer );
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

	WCConnection_close( connection );
	return 0;
}

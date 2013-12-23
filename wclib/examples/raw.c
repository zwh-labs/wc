#include <wc/WCPacket.h>
#include <wc/WCConnection.h>
#include <wc/WCConfiguration.h>
#include <wc/WCConfiguration_ArgumentParser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>


int main( int argc, const char ** argv )
{
	fprintf( stderr, "Reading configuration ... " ); fflush( stderr );
	WCConfiguration * configuration = WCConfiguration_newFromArguments( argc, argv );
	if( !configuration )
		goto configuration_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Opening connection ... " ); fflush( stderr );
	WCConnection * connection = WCConnection_open( WCConfiguration_getDevicePath( configuration ) );
	if( !connection )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	WCPacket packet;
	WCPacket_RequestInfo_create( (WCPacket_RequestInfo*)&packet );
	WCConnection_write( connection, &packet );
	while( 1 )
	{
		if( WCConnection_read( connection, &packet ) <= 0)
		{
			fprintf( stderr, "Cannot read from connection\n" );
			break;
		}
		switch( packet.header.type )
		{
			case WCPACKET_MESSAGE_TYPE:
			{
				WCPacket_Message * message = (WCPacket_Message*)&packet;
				message->text[message->header.length] = 0;
				fprintf( stderr, "Received message:\"%s\"\n", message->text );
				break;
			}
			case WCPACKET_WHEEL_TYPE:
			{
				WCPacket_Wheel * wheel = (WCPacket_Wheel*)&packet;
				fprintf( stderr, "Received wheel:\tchannel=%d\terror=%d\tvalue=%d\n", wheel->channel, wheel->error, wheel->value );
				break;
			}
			default:
			{
				fprintf( stderr, "Received unknown packet type: %d\n", packet.header.type );
				break;
			}
		}
	}

	WCConnection_close( connection );
	return 0;

connection_failed:
	WCConfiguration_delete( configuration );
configuration_failed:
	fprintf( stderr, "failed\n" );
	return 1;
}

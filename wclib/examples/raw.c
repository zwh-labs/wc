#include <wc/Packet.h>
#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>
#include <wc/Configuration.h>
#include <wc/Configuration_ArgumentParser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>


int main( int argc, const char ** argv )
{
	fprintf( stderr, "Reading configuration ... " ); fflush( stderr );
	wcConfiguration * configuration = wcConfiguration_newFromArguments( argc, argv );
	if( !configuration )
		goto configuration_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Opening connection ... " ); fflush( stderr );
	wcConnection * connection = wcConnection_openFromConfiguration( configuration );
	if( !connection )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	wcPacket packet;
	wcPacket_RequestInfo_create( (wcPacket_RequestInfo*)&packet );
	wcConnection_write( connection, &packet );
	while( 1 )
	{
		if( wcConnection_read( connection, &packet ) <= 0)
		{
			fprintf( stderr, "Cannot read from connection\n" );
			break;
		}
		switch( packet.header.type )
		{
			case WC_PACKET_MESSAGE_TYPE:
			{
				wcPacket_Message * message = (wcPacket_Message*)&packet;
				message->text[message->header.length] = 0;
				fprintf( stderr, "Received message:\"%s\"\n", message->text );
				break;
			}
			case WC_PACKET_WHEEL_TYPE:
			{
				wcPacket_Wheel * wheel = (wcPacket_Wheel*)&packet;
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

	wcConnection_close( connection );
	return 0;

connection_failed:
	wcConfiguration_delete( configuration );
configuration_failed:
	fprintf( stderr, "failed\n" );
	return 1;
}

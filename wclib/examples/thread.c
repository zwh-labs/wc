#include <wc/WCConfiguration.h>
#include <wc/WCConfigurationArgumentParser.h>
#include <wc/WCConnection.h>
#include <wc/WCThread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifdef _WIN32
#include <windows.h>
void sleep( int sec )
{
	Sleep( sec * 1000 );
}
#endif


int main( int argc, const char ** argv )
{
	WCConfiguration * configuration = WCConfiguration_newFromArguments( argc, argv );
	if( configuration == NULL )
		goto configuration_failed;

	WCConnection * connection = WCConnection_open( WCConfiguration_getDevicePath( configuration) );
	if( connection == NULL )
		goto connection_failed;

	fprintf( stderr, "Starting thread\n" );
	WCThread * thread = WCThread_start( connection );
	if( thread == NULL )
		goto thread_failed;
	fprintf( stderr, "Started thread\n" );

	int cnt = 0;
	while( cnt < 5 )
	{
		sleep( 1 );
		for( int i = 0; i<WCThread_getWheelCount( thread ); i++ )
		{
			printf( "Wheel %d: %d\n", i, WCThread_retrieveWheel( thread, i ) );
		}
		cnt++;
	}

	fprintf( stderr, "Stopping thread\n" );
	WCThread_stop( thread );
	fprintf( stderr, "Stopped thread\n" );

	WCConnection_close( connection );
	WCConfiguration_delete( configuration );
	return 0;

thread_failed:
	fprintf( stderr, "Cannot start thread on connection\n" );
	WCConnection_close( connection );
connection_failed:
	fprintf( stderr, "Cannot open connection\n" );
	WCConfiguration_delete( configuration );
configuration_failed:
	fprintf( stderr, "Cannot create configuration\n" );
	return 1;
}

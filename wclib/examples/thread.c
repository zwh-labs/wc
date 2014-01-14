#include <wc/WCConfiguration.h>
#include <wc/WCConfiguration_ArgumentParser.h>
#include <wc/WCConnection.h>
#include <wc/WCThread.h>
#include <wc/WCWheelMovement.h>
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
	fprintf( stderr, "Reading configuration ... " ); fflush( stderr );
	WCConfiguration * configuration = WCConfiguration_newFromArguments( argc, argv );
	if( !configuration )
		goto configuration_failed;
	fprintf( stderr, "done\n" );
	WCConfiguration_fprint( stderr, configuration );

	fprintf( stderr, "Opening connection ... " ); fflush( stderr );
	WCConnection * connection = WCConnection_open( WCConfiguration_getDevicePath( configuration ) );
	if( !connection )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Starting thread on connection ... " ); fflush( stderr );
	WCThread * thread = WCThread_start( connection );
	if( !thread )
		goto thread_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Running for 10 seconds\n" );
	int cnt = 0;
	while( cnt < 10 )
	{
		sleep( 1 );
		for( unsigned int i = 0; i<WCThread_getWheelCount( thread ); i++ )
		{
			WCWheelMovement wm = WCThread_retrieveWheelMovement( thread, i );
			printf( "Wheel:\tchannel=%d\terror=%d\tvalue=%d\n", i, WCWheelMovement_getError( &wm ), WCWheelMovement_getIncrements( &wm ) );
		}
		cnt++;
	}

	fprintf( stderr, "Stopping thread ... " ); fflush( stderr );
	if( !WCThread_stop( thread ) )
		goto thread_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Closing connection ... " ); fflush( stderr );
	if( !WCConnection_close( connection ) )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	WCConfiguration_delete( configuration );
	return 0;

thread_failed:
	WCConnection_close( connection );
connection_failed:
	WCConfiguration_delete( configuration );
configuration_failed:
	fprintf( stderr, "failed\n" );
	return 1;
}

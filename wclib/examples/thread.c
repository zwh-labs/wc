#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>
#include <wc/Configuration_ArgumentParser.h>
#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>
#include <wc/Thread.h>
#include <wc/WheelMovement.h>
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
	wcConfiguration * configuration = wcConfiguration_newFromArguments( argc, argv );
	if( !configuration )
		goto configuration_failed;
	fprintf( stderr, "done\n" );
	wcConfiguration_fprint( stderr, configuration );

	fprintf( stderr, "Opening connection ... " ); fflush( stderr );
	wcConnection * connection = wcConnection_openFromConfiguration( configuration );
	if( !connection )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Starting thread on connection ... " ); fflush( stderr );
	wcThread * thread = wcThread_start( connection );
	if( !thread )
		goto thread_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Running for 10 seconds\n" );
	int cnt = 0;
	while( cnt < 10 )
	{
		sleep( 1 );
		for( unsigned int i = 0; i<wcThread_getWheelCount( thread ); i++ )
		{
			wcWheelMovement wm = wcThread_retrieveWheelMovement( thread, i );
			printf( "Wheel:\tchannel=%d\terror=%d\tvalue=%d\n", i, wcWheelMovement_getError( &wm ), wcWheelMovement_getIncrements( &wm ) );
		}
		cnt++;
	}

	fprintf( stderr, "Stopping thread ... " ); fflush( stderr );
	if( !wcThread_stop( thread ) )
		goto thread_failed;
	fprintf( stderr, "done\n" );

	fprintf( stderr, "Closing connection ... " ); fflush( stderr );
	if( !wcConnection_close( connection ) )
		goto connection_failed;
	fprintf( stderr, "done\n" );

	wcConfiguration_delete( configuration );
	return 0;

thread_failed:
	wcConnection_close( connection );
connection_failed:
	wcConfiguration_delete( configuration );
configuration_failed:
	fprintf( stderr, "failed\n" );
	return 1;
}

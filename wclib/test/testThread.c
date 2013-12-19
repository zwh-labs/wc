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

	WCThread * thread = WCThread_start( connection );

	fprintf( stderr, "Starting thread\n" );

	if( thread == NULL )
	{
		fprintf( stderr, "Cannot start thread on connection\n" );
		exit( 1 );
	}

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
	return 0;
}

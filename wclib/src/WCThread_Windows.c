#include <wc/WCConnection.h>

#include <windows.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct WCThread
{
	HANDLE thread;
	DWORD threadID;

	HANDLE shutdownMutex;
	bool shutdown;

	HANDLE connectionMutex;
	WCConnection * connection;

	HANDLE wheelsMutex;
	int numWheels;
	int * wheels;
} WCThread;


void handleWheel( WCThread * thread, WCPacket_Wheel * wheel )
{
	WaitForSingleObject( thread->wheelsMutex, INFINITE );
	if( thread->numWheels <= wheel->channel )
	{
		fprintf( stderr, "Resizing number of tracked channels from %d to %d\n", thread->numWheels, wheel->channel + 1 );
		thread->wheels = realloc( thread->wheels, sizeof(int) * (wheel->channel + 1) );
		thread->numWheels = wheel->channel + 1;
		thread->wheels[wheel->channel] = 0;
	}
	thread->wheels[wheel->channel] += wheel->value;
	ReleaseMutex( thread->wheelsMutex )
}


static DWORD connectionThread( LPVOID data )
{
	WCThread * thread = (WCThread*) data;
	bool shutdown = false;
	while( !shutdown )
	{
		uint8_t buffer[WCPACKET_MAXSIZE];
		WCPacket_Header * header = (WCPacket_Header*)buffer;
		
		WaitForSingleObject( thread->connectionMutex, INFINITE );
		WCConnection_read( thread->connection, (WCPacket*)buffer );
		ReleaseMutex( thread->connectionMutex )
		
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
				handleWheel( thread, wheel );
//				fprintf( stderr, "Received wheel:\tchannel=%d\terror=%d\tvalue=%d\n", wheel->channel, wheel->error, wheel->value );
				break;
			}
			default:
			{
				fprintf( stderr, "Received unknown packet type: %d\n", header->type );
				break;
			}
		}
		
		WaitForSingleObject( thread->shutdownMutex, INFINITE );
		shutdown = thread->shutdown;
		ReleaseMutex( thread->shutdownMutex );
	}
	return NULL;
}


WCThread * WCThread_start( WCConnection * connection )
{
	WCThread * thread = (WCThread*) malloc( sizeof(WCThread) );
	
	thread->shutdown = false;
	thread->connection = connection;
	thread->numWheels = 0;
	thread->wheels = NULL;

	thread->connectionMutex = CreateMutex( NULL, FALSE, NULL );
	thread->wheelsMutex = CreateMutex( NULL, FALSE, NULL );
	thread->shutdownMutex = CreateMutex( NULL, FALSE, NULL );
	if( !thread->connectionMutex || !thread->wheelsMutex || !thread->shutdownMutex )
	{
		fprintf( stderr, "CreateMutex failed\n" );
		free( thread );
		return NULL;
	}

	thread->thread = CreateThread( 
		NULL,       // default security attributes
		0,          // default stack size
		connectionThread,
		thread,     // no thread function arguments
		0,          // default creation flags
		&thread->threadID); // receive thread identifier
	
	if( !thread->thread )
	{
		fprintf( stderr, "CreateThread failed\n" );
		CloseHandle( thread->connectionMutex );
		CloseHandle( thread->wheelsMutex );
		CloseHandle( thread->shutdownMutex );
		free( thread );
		return NULL;
	}
	return thread;
}


int WCThread_stop( WCThread * thread )
{
	WaitForSingleObject( thread->shutdownMutex, INFINITE );
	thread->shutdown = true;
	ReleaseMutex( thread->shutdownMutex )

	WaitForSingleObject( thread->thread, INFINITE );
	CloseHandle( thread->connectionMutex );
	CloseHandle( thread->wheelsMutex );
	CloseHandle( thread->shutdownMutex );
	free( thread->wheels );
	free( thread );
	return 0;
}


int WCThread_retrieveWheel( WCThread * thread, int channel )
{
	int wheel = 0;
	WaitForSingleObject( thread->wheelsMutex, INFINITE );
	if( channel >= 0 && channel < thread->numWheels )
	{
		wheel = thread->wheels[channel];
		thread->wheels[channel] = 0;
	}
	ReleaseMutex( thread->wheelsMutex )
	return wheel;
}


int WCThread_getWheelCount( WCThread * thread )
{
	int wheelCount = 0;
	WaitForSingleObject( thread->wheelsMutex, INFINITE );
	wheelCount = thread->numWheels;
	ReleaseMutex( thread->wheelsMutex )
	return wheelCount;
}

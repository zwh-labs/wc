#include "WCThread_Thread.h"
#include "WCThread_Mutex.h"

#include <wc/WCConnection.h>
#include <wc/WCWheelMovement.h>
#include <wc/WCWheelMovement_Configuration.h>
#include <wc/WCWheelMovement_Packet.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct WCThread
{
	WCThread_Thread * thread;

	WCThread_Mutex * shutdownMutex;
	bool shutdown;

	WCThread_Mutex * connectionMutex;
	WCConnection * connection;

	WCThread_Mutex * wheelsMutex;
	unsigned int numWheels;
	WCWheelMovement * wheelMovements;
} WCThread;


static void handleWheel( WCThread * thread, WCPacket_Wheel * pw )
{
	WCThread_Mutex_lock( thread->wheelsMutex );
		if( thread->numWheels <= pw->channel )
		{
			fprintf( stderr, "Increasing number of tracked wheels from %d to %d\n", thread->numWheels, pw->channel + 1 );
			thread->wheelMovements = realloc( thread->wheelMovements, sizeof(WCWheelMovement) * (pw->channel + 1) );
			WCWheelMovement_createFromPacket( &(thread->wheelMovements[thread->numWheels]), pw );
			thread->numWheels = pw->channel + 1;
		}
		else
		{
			WCWheelMovement_accumulateFromPacket( &(thread->wheelMovements[pw->channel]), pw );
		}
	WCThread_Mutex_unlock( thread->wheelsMutex );
}


static void connectionThread( void * data )
{
	WCThread * thread = (WCThread*) data;
	bool shutdown = false;
	while( !shutdown )
	{
		WCPacket packet;
		int read;

		WCThread_Mutex_lock( thread->connectionMutex );
			read = WCConnection_read( thread->connection, &packet );
		WCThread_Mutex_unlock( thread->connectionMutex );

		if( read <= 0)
		{
			fprintf( stderr, "Cannot read from connection\n" );
			WCThread_Mutex_lock( thread->shutdownMutex );
				thread->shutdown = true;
			WCThread_Mutex_unlock( thread->shutdownMutex );
			break;
		}

		switch( packet.header.type )
		{
			case WCPACKET_MESSAGE_TYPE:
			{
				WCPacket_Message * message = (WCPacket_Message*)(&packet);
				message->text[message->header.length] = 0;
				fprintf( stderr, "Received message:\"%s\"\n", message->text );
				break;
			}
			case WCPACKET_WHEEL_TYPE:
			{
				WCPacket_Wheel * wheel = (WCPacket_Wheel*)(&packet);
				handleWheel( thread, wheel );
//				fprintf( stderr, "Received wheel:\tchannel=%d\terror=%d\tvalue=%d\n", wheel->channel, wheel->error, wheel->value );
				break;
			}
			default:
			{
				fprintf( stderr, "Received unknown packet type: %d\n", packet.header.type );
				break;
			}
		}

		WCThread_Mutex_lock( thread->shutdownMutex );
			shutdown = thread->shutdown;
		WCThread_Mutex_unlock( thread->shutdownMutex );
	}
}


WCThread * WCThread_start( WCConnection * connection )
{
	WCThread * thread = (WCThread*) malloc( sizeof(WCThread) );

	thread->shutdown = false;
	thread->connection = connection;
	thread->numWheels = 0;
	thread->wheelMovements = NULL;

	thread->connectionMutex = WCThread_Mutex_new();
	thread->wheelsMutex = WCThread_Mutex_new();
	thread->shutdownMutex = WCThread_Mutex_new();

	if( !thread->connectionMutex || !thread->wheelsMutex || !thread->shutdownMutex )
	{
		fprintf( stderr, "WCThread_Mutex_new failed\n" );
		free( thread );
		return NULL;
	}

	thread->thread = WCThread_Thread_new( connectionThread, thread );

	if( !thread->thread )
	{
		fprintf( stderr, "WCThread_Thread_new failed\n" );
		WCThread_Thread_delete( thread->thread );
		WCThread_Mutex_delete( thread->connectionMutex );
		WCThread_Mutex_delete( thread->wheelsMutex );
		WCThread_Mutex_delete( thread->shutdownMutex );
		free( thread );
		return NULL;
	}
	return thread;
}


bool WCThread_stop( WCThread * thread )
{
	WCThread_Mutex_lock( thread->shutdownMutex );
		thread->shutdown = true;
	WCThread_Mutex_unlock( thread->shutdownMutex );

	WCThread_Thread_join( thread->thread );
	WCThread_Thread_delete( thread->thread );

	WCThread_Mutex_delete( thread->connectionMutex );
	WCThread_Mutex_delete( thread->wheelsMutex );
	WCThread_Mutex_delete( thread->shutdownMutex );

	free( thread->wheelMovements );
	free( thread );

	return true;
}


WCWheelMovement WCThread_retrieveWheelMovement( WCThread * thread, unsigned int index )
{
	WCWheelMovement wheel;
	WCWheelMovement_create( &wheel, index );
	WCThread_Mutex_lock( thread->wheelsMutex );
		if( index < thread->numWheels )
		{
			wheel = thread->wheelMovements[index];
			WCWheelMovement_reset( &(thread->wheelMovements[index]) );
		}
	WCThread_Mutex_unlock( thread->wheelsMutex );
	return wheel;
}


int WCThread_getWheelCount( WCThread * thread )
{
	int wheelCount = 0;
	WCThread_Mutex_lock( thread->wheelsMutex );
		wheelCount = thread->numWheels;
	WCThread_Mutex_unlock( thread->wheelsMutex );
	return wheelCount;
}

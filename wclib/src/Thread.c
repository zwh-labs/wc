#include <wc/Thread.h>

#include "Thread_Thread.h"
#include "Thread_Mutex.h"

#include <wc/Connection.h>
#include <wc/WheelMovement.h>
#include <wc/WheelMovement_Configuration.h>
#include <wc/WheelMovement_Packet.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


struct _wcThread
{
	wcThread_Thread * thread;

	wcThread_Mutex * shutdownMutex;
	bool shutdown;

	wcThread_Mutex * connectionMutex;
	wcConnection * connection;

	wcThread_Mutex * wheelsMutex;
	unsigned int numWheels;
	wcWheelMovement * wheelMovements;
};


static void handleWheel( wcThread * thread, wcPacket_Wheel * pw )
{
	wcThread_Mutex_lock( thread->wheelsMutex );
		if( thread->numWheels <= pw->channel )
		{
//			fprintf( stderr, "Increasing number of tracked wheels from %d to %d\n", thread->numWheels, pw->channel + 1 );
			thread->wheelMovements = (wcWheelMovement*) realloc( thread->wheelMovements, sizeof(wcWheelMovement) * (pw->channel + 1) );
			wcWheelMovement_createFromPacket( &(thread->wheelMovements[thread->numWheels]), pw );
			thread->numWheels = pw->channel + 1;
		}
		else
		{
			wcWheelMovement_accumulateFromPacket( &(thread->wheelMovements[pw->channel]), pw );
		}
	wcThread_Mutex_unlock( thread->wheelsMutex );
}


static void connectionThread( void * data )
{
	wcThread * thread = (wcThread*) data;
	bool shutdown = false;
	while( !shutdown )
	{
		wcPacket packet;
		int read;

		wcThread_Mutex_lock( thread->connectionMutex );
			read = wcConnection_read( thread->connection, &packet );
		wcThread_Mutex_unlock( thread->connectionMutex );

		if( read <= 0)
		{
			fprintf( stderr, "Cannot read from connection\n" );
			wcThread_Mutex_lock( thread->shutdownMutex );
				thread->shutdown = true;
			wcThread_Mutex_unlock( thread->shutdownMutex );
			break;
		}

		switch( packet.header.type )
		{
			case WC_PACKET_MESSAGE_TYPE:
			{
				wcPacket_Message * message = (wcPacket_Message*)(&packet);
				message->text[message->header.length] = 0;
				fprintf( stderr, "Received message:\"%s\"\n", message->text );
				break;
			}
			case WC_PACKET_WHEEL_TYPE:
			{
				wcPacket_Wheel * wheel = (wcPacket_Wheel*)(&packet);
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

		wcThread_Mutex_lock( thread->shutdownMutex );
			shutdown = thread->shutdown;
		wcThread_Mutex_unlock( thread->shutdownMutex );
	}
}


wcThread * wcThread_start( wcConnection * connection )
{
	wcThread * thread = (wcThread*) malloc( sizeof(wcThread) );

	thread->shutdown = false;
	thread->connection = connection;
	thread->numWheels = 0;
	thread->wheelMovements = NULL;

	thread->connectionMutex = wcThread_Mutex_new();
	thread->wheelsMutex = wcThread_Mutex_new();
	thread->shutdownMutex = wcThread_Mutex_new();

	if( !thread->connectionMutex || !thread->wheelsMutex || !thread->shutdownMutex )
	{
		fprintf( stderr, "wcThread_Mutex_new failed\n" );
		free( thread );
		return NULL;
	}

	thread->thread = wcThread_Thread_new( connectionThread, thread );

	if( !thread->thread )
	{
		fprintf( stderr, "wcThread_Thread_new failed\n" );
		wcThread_Thread_delete( thread->thread );
		wcThread_Mutex_delete( thread->connectionMutex );
		wcThread_Mutex_delete( thread->wheelsMutex );
		wcThread_Mutex_delete( thread->shutdownMutex );
		free( thread );
		return NULL;
	}
	return thread;
}


bool wcThread_stop( wcThread * thread )
{
	if( !thread )
		return false;

	wcThread_Mutex_lock( thread->shutdownMutex );
		thread->shutdown = true;
	wcThread_Mutex_unlock( thread->shutdownMutex );

	wcThread_Thread_join( thread->thread );
	wcThread_Thread_delete( thread->thread );

	wcThread_Mutex_delete( thread->connectionMutex );
	wcThread_Mutex_delete( thread->wheelsMutex );
	wcThread_Mutex_delete( thread->shutdownMutex );

	free( thread->wheelMovements );
	free( thread );

	return true;
}


wcWheelMovement wcThread_retrieveWheelMovement( wcThread * thread, unsigned int index )
{
	wcWheelMovement wheel;
	wcWheelMovement_create( &wheel, index );
	wcThread_Mutex_lock( thread->wheelsMutex );
		if( index < thread->numWheels )
		{
			wheel = thread->wheelMovements[index];
			wcWheelMovement_reset( &(thread->wheelMovements[index]) );
		}
	wcThread_Mutex_unlock( thread->wheelsMutex );
	return wheel;
}


unsigned int wcThread_getWheelCount( wcThread * thread )
{
	unsigned int wheelCount = 0;
	wcThread_Mutex_lock( thread->wheelsMutex );
		wheelCount = thread->numWheels;
	wcThread_Mutex_unlock( thread->wheelsMutex );
	return wheelCount;
}

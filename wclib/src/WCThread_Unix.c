#include <wc/WCConnection.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct WCThread
{
	pthread_t thread;

	pthread_mutex_t shutdownMutex;
	bool shutdown;

	pthread_mutex_t connectionMutex;
	WCConnection * connection;

	pthread_mutex_t wheelsMutex;
	int numWheels;
	int * wheels;
} WCThread;


void handleWheel( WCThread * thread, WCPacket_Wheel * wheel )
{
	pthread_mutex_lock( &(thread->wheelsMutex) );
		if( thread->numWheels <= wheel->channel )
		{
			fprintf( stderr, "Resizing number of tracked channels from %d to %d\n", thread->numWheels, wheel->channel + 1 );
			thread->wheels = realloc( thread->wheels, sizeof(int) * (wheel->channel + 1) );
			thread->numWheels = wheel->channel + 1;
			thread->wheels[wheel->channel] = 0;
		}
		thread->wheels[wheel->channel] += wheel->value;
	pthread_mutex_unlock( &(thread->wheelsMutex) );
}


static void * connectionThread( void * data )
{
	WCThread * thread = (WCThread*) data;
	bool shutdown = false;
	while( !shutdown )
	{
		uint8_t buffer[WCPACKET_MAXSIZE];
		WCPacket_Header * header = (WCPacket_Header*)buffer;

		pthread_mutex_lock( &(thread->connectionMutex) );
		WCConnection_read( thread->connection, (WCPacket*)buffer );
		pthread_mutex_unlock( &(thread->connectionMutex) );

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

		pthread_mutex_lock( &(thread->shutdownMutex) );
		shutdown = thread->shutdown;
		pthread_mutex_unlock( &(thread->shutdownMutex) );
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

	if( pthread_mutex_init( &(thread->connectionMutex), NULL ) != 0 )
		goto pthread_mutex_init_failed;
	
	if( pthread_mutex_init( &(thread->wheelsMutex), NULL ) != 0 )
		goto pthread_mutex_init_failed;
	
	if( pthread_mutex_init( &(thread->shutdownMutex), NULL ) != 0 )
		goto pthread_mutex_init_failed;

	if( pthread_create( &(thread->thread), NULL, connectionThread, thread ) != 0 )
	{
		fprintf( stderr, "pthread_create failed\n" );
		pthread_mutex_destroy( &(thread->connectionMutex) );
		pthread_mutex_destroy( &(thread->wheelsMutex) );
		pthread_mutex_destroy( &(thread->shutdownMutex) );
		free( thread );
		return NULL;
	}
	return thread;

pthread_mutex_init_failed:
	fprintf( stderr, "pthread_mutex_init failed\n" );
	free( thread );
	return NULL;
}


int WCThread_stop( WCThread * thread )
{
	pthread_mutex_lock( &(thread->shutdownMutex) );
		thread->shutdown = true;
	pthread_mutex_unlock( &(thread->shutdownMutex) );

	pthread_join( thread->thread, NULL );
	pthread_mutex_destroy( &(thread->connectionMutex) );
	pthread_mutex_destroy( &(thread->wheelsMutex) );
	pthread_mutex_destroy( &(thread->shutdownMutex) );
	free( thread->wheels );
	free( thread );
	return 0;
}


int WCThread_retrieveWheel( WCThread * thread, int channel )
{
	int wheel = 0;
	pthread_mutex_lock( &(thread->wheelsMutex) );
		if( channel >= 0 && channel < thread->numWheels )
		{
			wheel = thread->wheels[channel];
			thread->wheels[channel] = 0;
		}
	pthread_mutex_unlock( &(thread->wheelsMutex) );
	return wheel;
}


int WCThread_getWheelCount( WCThread * thread )
{
	int wheelCount = 0;
	pthread_mutex_lock( &(thread->wheelsMutex) );
		wheelCount = thread->numWheels;
	pthread_mutex_unlock( &(thread->wheelsMutex) );
	return wheelCount;
}

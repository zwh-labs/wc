#include <wc/WCPacket.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


typedef struct WCConnection
{
	int fd;
	struct termios initialTio;
	struct termios tio;
} WCConnection;


WCConnection * WCConnection_open( const char * file )
{
	WCConnection * connection = (WCConnection*) malloc( sizeof(WCConnection) );

	connection->fd = open( file, O_RDWR | O_NOCTTY );
	if( connection->fd < 0 )
	{
		free( connection );
		return NULL;
	}

	tcgetattr( connection->fd, &(connection->initialTio) );

	memset( &(connection->tio), 0, sizeof(connection->tio) );
	connection->tio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	connection->tio.c_iflag = IGNPAR;
	connection->tio.c_oflag = 0;
	connection->tio.c_lflag = 0;
	connection->tio.c_cc[VTIME] = 0;
	connection->tio.c_cc[VMIN] = 1;
	tcflush( connection->fd, TCIFLUSH );
	tcsetattr( connection->fd, TCSANOW, &connection->tio );
	
	return connection;
}


int WCConnection_close( WCConnection * connection )
{
	tcsetattr( connection->fd, TCSANOW, &(connection->initialTio) );
	int ret = close( connection->fd );
	free( connection );
	return ret;
}


// just a wrapper that won't return until "length" bytes are read or an error occurs - be sure to use a blocking "fd"
static ssize_t readBytes( int fd, void * buffer, size_t length )
{
	size_t bytesRead = 0;
	while( bytesRead < length )
	{
		ssize_t ret = read( fd, ((char*)buffer) + bytesRead, length - bytesRead );
		if( ret <= 0 )
			return ret;
		bytesRead += ret;
	}
	return bytesRead;
}


int WCConnection_read( WCConnection * connection, WCPacket * buffer )
{
	int ret;
	WCPacket_Header * header = (WCPacket_Header*)buffer;

	// header
	ret = readBytes( connection->fd, buffer, sizeof(WCPacket_Header) );
	if( ret != sizeof(WCPacket_Header) )
		return ret; // return error or size read

	// remaining packet
	ret = readBytes( connection->fd, ((char*)buffer) + sizeof(WCPacket_Header), header->length );
	if( ret < 0 )
		return ret; // return error
	else
		return ret + sizeof(WCPacket_Header); // return size read + size read for header
}


int WCConnection_write( WCConnection * connection, const WCPacket * buffer )
{
	WCPacket_Header * header = (WCPacket_Header*)buffer;
	return write( connection->fd, buffer, header->length + sizeof(WCPacket_Header) );
}

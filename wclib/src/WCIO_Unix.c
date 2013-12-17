#include <wc/WCPacket.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


int WCIO_open( const char * file )
{
	int fd = open( file, O_RDWR | O_NOCTTY );
	if( fd < 0 )
		return fd;

	struct termios newtio;
	memset( &newtio, 0, sizeof(newtio) );
	newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 1;
	tcflush( fd, TCIFLUSH );
	tcsetattr( fd, TCSANOW, &newtio );
	
	return fd;
}


int WCIO_close( int fd )
{
	return close( fd );
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


int WCIO_read( int fd, WCPacket * buffer )
{
	int ret;
	WCPacket_Header * header = (WCPacket_Header*)buffer;

	// header
	ret = readBytes( fd, buffer, sizeof(WCPacket_Header) );
	if( ret != sizeof(WCPacket_Header) )
		return ret; // return error or size read

	// remaining packet
	ret = readBytes( fd, ((char*)buffer) + sizeof(WCPacket_Header), header->length );
	if( ret < 0 )
		return ret; // return error
	else
		return ret + sizeof(WCPacket_Header); // return size read + size read for header
}


int WCIO_write( int fd, const WCPacket * buffer )
{
	WCPacket_Header * header = (WCPacket_Header*)buffer;
	return write( fd, buffer, header->length + sizeof(WCPacket_Header) );
}

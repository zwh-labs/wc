#include <wc/WCIO_Unix.h>
#include <wc/WCPacket.h>
#include <unistd.h>
#include <fcntl.h>


int WCIO_Unix_open( const char * file )
{
	return open( file, O_RDWR | O_NOCTTY );
}


int WCIO_Unix_close( int fd )
{
	return close( fd );
}


int WCIO_Unix_read( int fd, void * buffer )
{
	int ret;
	WCPacket_Header * header = (WCPacket_Header*)buffer;

	// header
	ret = read( fd, buffer, sizeof(WCPacket_Header) );
	if( ret != sizeof(WCPacket_Header) )
		return ret; // return error or size read

	// remaining packet
	ret = read( fd, ((char*)buffer) + sizeof(WCPacket_Header), header->length );
	if( ret < 0 )
		return ret; // return error
	else
		return ret + sizeof(WCPacket_Header); // return size read + size read for header
}


int WCIO_Unix_write( int fd, const void * buffer )
{
	WCPacket_Header * header = (WCPacket_Header*)buffer;
	return write( fd, buffer, header->length + sizeof(WCPacket_Header) );
}

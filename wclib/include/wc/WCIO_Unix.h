#ifndef _WCIO_UNIX_H_
#define _WCIO_UNIX_H_


#include <wc/WCPacket.h>


int WCIO_Unix_open( const char * file );

int WCIO_Unix_close( int fd );

int WCIO_Unix_read( int fd, WCPacket * buffer );

int WCIO_Unix_write( int fd, const WCPacket * buffer );


#endif

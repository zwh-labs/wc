#ifndef _WCIO_H_
#define _WCIO_H_


#include <wc/WCPacket.h>


int WCIO_open( const char * file );

int WCIO_close( int fd );

int WCIO_read( int fd, WCPacket * buffer );

int WCIO_write( int fd, const WCPacket * buffer );


#endif

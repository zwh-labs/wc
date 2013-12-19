#ifndef _WCCONNECTION_H_
#define _WCCONNECTION_H_


#include <wc/WCPacket.h>


typedef struct WCConnection WCConnection;


WCConnection * WCConnection_open( const char * file );
int WCConnection_close( WCConnection * controller );

int WCConnection_read( WCConnection * controller, WCPacket * buffer );
int WCConnection_write( WCConnection * controller, const WCPacket * buffer );


#endif

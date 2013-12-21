#ifndef _WCCONNECTION_H_
#define _WCCONNECTION_H_


#include <wc/WCPacket.h>

#include <stdbool.h>


typedef struct WCConnection WCConnection;


WCConnection * WCConnection_open( const char * file );
bool WCConnection_close( WCConnection * controller );

int WCConnection_read( WCConnection * controller, WCPacket * packet );
int WCConnection_write( WCConnection * controller, const WCPacket * packet );


#endif

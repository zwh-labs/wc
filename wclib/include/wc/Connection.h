#ifndef _WC_CONNECTION_H_
#define _WC_CONNECTION_H_


#include <wc/Packet.h>

#include <stdbool.h>


typedef struct _wcConnection wcConnection;


wcConnection * wcConnection_open( const char * file );
bool wcConnection_close( wcConnection * controller );

int wcConnection_read( wcConnection * controller, wcPacket * packet );
int wcConnection_write( wcConnection * controller, const wcPacket * packet );


#endif

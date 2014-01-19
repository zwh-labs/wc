#ifndef _WC_CONNECTION_H_
#define _WC_CONNECTION_H_


#include "wc_export.h"
#include <wc/Packet.h>

#include <stdbool.h>


typedef struct _wcConnection wcConnection;


WC_EXPORT wcConnection * wcConnection_open( const char * file );
WC_EXPORT bool wcConnection_close( wcConnection * controller );

WC_EXPORT int wcConnection_read( wcConnection * controller, wcPacket * packet );
WC_EXPORT int wcConnection_write( wcConnection * controller, const wcPacket * packet );


#endif

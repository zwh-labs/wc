#include <wc/Connection_Configuration.h>


wcConnection* wcConnection_openFromConfiguration( const wcConfiguration * configuration )
{
	return wcConnection_open( wcConfiguration_getDevicePath( configuration ) );
}

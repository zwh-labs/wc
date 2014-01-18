#include <wc/Connection.h>
#include <wc/Packet.h>

#include <stdbool.h>
#include <errno.h>
#include <windows.h>


struct _wcConnection
{
	HANDLE handle;
};


wcConnection * wcConnection_open( const char * file )
{
	wcConnection * connection = (wcConnection*) malloc( sizeof(wcConnection) );

	connection->handle = CreateFile( file, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL );
	if( connection->handle == INVALID_HANDLE_VALUE )
	{
		free( connection );
		return NULL;
	}

	COMMCONFIG cfg;
	DWORD cfgSize = sizeof(COMMCONFIG);
	GetCommConfig( connection->handle, &cfg, &cfgSize );
	cfg.dcb.BaudRate = 115200;
	cfg.dcb.fBinary = TRUE;
	cfg.dcb.fParity = FALSE;
	cfg.dcb.fOutxCtsFlow = FALSE;
	cfg.dcb.fOutxDsrFlow = FALSE;
	cfg.dcb.fDtrControl = DTR_CONTROL_ENABLE;
	cfg.dcb.fDsrSensitivity = FALSE;
	cfg.dcb.fTXContinueOnXoff = TRUE;
	cfg.dcb.fOutX = FALSE;
	cfg.dcb.fInX = FALSE;
	cfg.dcb.fErrorChar = FALSE;
	cfg.dcb.fNull = FALSE;
	cfg.dcb.fRtsControl = RTS_CONTROL_ENABLE;
	cfg.dcb.fAbortOnError = FALSE;
	cfg.dcb.XonLim = 0x8000;
	cfg.dcb.XoffLim = 20;
	cfg.dcb.ByteSize = 8;
	cfg.dcb.Parity = NOPARITY;
	cfg.dcb.StopBits = ONESTOPBIT;
	SetCommConfig( connection->handle, &cfg, cfgSize );

	COMMTIMEOUTS timeout;
	GetCommTimeouts( connection->handle, &timeout );
	timeout.ReadIntervalTimeout = 0;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.ReadTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts( connection->handle, &timeout );

	return connection;
}


bool wcConnection_close( wcConnection * connection )
{
	if( !connection )
		return false;
	BOOL ret = CloseHandle( connection->handle );
	free( connection );
	return ret;
}


int wcConnection_read( wcConnection * connection, wcPacket * packet )
{
	DWORD read = 0;
	BOOL ret = ReadFile( connection->handle, &(packet->header), sizeof(wcPacket_Header), &read, NULL);
	if( !ret )
		return -1;
	if( read != sizeof(wcPacket_Header) )
		return read;

	read = 0;
	ret = ReadFile( connection->handle, packet->_data, packet->header.length, &read, NULL);
	if( !ret )
		return -1;
	else
		return read + sizeof(wcPacket_Header);
}


int wcConnection_write( wcConnection * connection, const wcPacket * packet )
{
	DWORD written = 0;
	BOOL ret = WriteFile( connection->handle, packet, wcPacket_size( packet ), &written, NULL);
	if( !ret )
		return -1;
	else
		return written;
}

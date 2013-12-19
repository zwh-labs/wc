#include <wc/WCPacket.h>
#include <errno.h>
#include <windows.h>


typedef struct WCConnection
{
	HANDLE handle;
} WCConnection;


WCConnection * WCConnection_open( const char * file )
{
	WCConnection * connection = (WCConnection*) malloc( sizeof(WCConnection) );

	connection->handle = CreateFile( file, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL );
	if( connection->handle == INVALID_HANDLE_VALUE )
	{
		free( connection );
		return NULL;
	}

	//TODO: check this
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

	//TODO: check this
	COMMTIMEOUTS timeout;
	GetCommTimeouts( connection->handle, &timeout );
	timeout.ReadIntervalTimeout = MAXDWORD;
	timeout.ReadTotalTimeoutMultiplier = MAXDWORD;
	timeout.ReadTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutConstant = 1000;
	timeout.WriteTotalTimeoutMultiplier = 1;
	SetCommTimeouts( connection->handle, &timeout );

	return connection;
}


int WCConnection_close( WCConnection * connection )
{
	BOOL ret = CloseHandle( connection->handle );
	free( connection );
	if( ret )
		return 0;
	else
		return -1;
}


int WCConnection_read( WCConnection * connection, WCPacket * buffer )
{
	DWORD read = 0;
	BOOL ret = ReadFile( connection->handle, buffer, sizeof(WCPacket_Header), &read, NULL);
	if( !ret )
		return -1;
	if( read != sizeof(WCPacket_Header) )
		return read;

	WCPacket_Header * header = (WCPacket_Header*)buffer;
	read = 0;
	ret = ReadFile( connection->handle, ((char*)buffer) + sizeof(WCPacket_Header), header->length, &read, NULL);
	if( !ret )
		return -1;
	else
		return read + sizeof(WCPacket_Header);
}


int WCConnection_write( WCConnection * connection, const WCPacket * buffer )
{
	WCPacket_Header * header = (WCPacket_Header*)buffer;
	DWORD written = 0;
	BOOL ret = WriteFile( connection->handle, buffer, header->length + sizeof(WCPacket_Header), &written, NULL);
	if( !ret )
		return -1;
	else
		return written;
}

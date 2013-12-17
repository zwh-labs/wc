#include <wc/WCPacket.h>
#include <windows.h>


#define MAXHANDLES 64

static HANDLE handles[MAXHANDLES] = {INVALID_HANDLE_VALUE};


static int getFreeHandleIndex(void)
{
	for( int i=0; i<MAXHANDLES; i++ )
	{
		if( handles[i] == INVALID_HANDLE_VALUE )
			return i;
	}
	return -1;
}


int WCIO_open( const char * file )
{
	int fd = getFreeHandleIndex();
	if( fd < 0 )
		return -1;

	handles[fd] = CreateFile( file, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, NULL );
	if( handles[fd] == INVALID_HANDLE_VALUE )
		return -1; // CreateFile failed

	//TODO: check this
	GetCommConfig(handles[fd], &cfg, &n);
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
	SetCommConfig(handles[fd], &cfg, n);

	//TODO: check this
	GetCommTimeouts(handles[fd], &timeout);
	timeout.ReadIntervalTimeout = 250;
	timeout.ReadTotalTimeoutMultiplier = 1;
	timeout.ReadTotalTimeoutConstant = 500;
	timeout.WriteTotalTimeoutConstant = 2500;
	timeout.WriteTotalTimeoutMultiplier = 1;
	SetCommTimeouts(handles[fd], &timeout);

	return fd;
}


int WCIO_close( int fd )
{
	if( CloseHandle( handles[fd] ) )
		return 0;
	else
		return -1;
}


int WCIO_read( int fd, WCPacket * buffer )
{
	DWORD read = 0;
	BOOL ret = ReadFile( handles[fd], buffer, sizeof(WCPacket_Header), &read, NULL);
	if( !ret )
		return -1;
	if( read != sizeof(WCPacket_Header) )
		return read;

	WCPacket_Header * header = (WCPacket_Header*)buffer;
	read = 0;
	ret = ReadFile( handles[fd], ((char*)buffer) + sizeof(WCPacket_Header), header->length, &read, NULL);
	if( !ret )
		return -1;
	else
		return read + sizeof(WCPacket_Header);
}


int WCIO_write( int fd, const WCPacket * buffer )
{
	WCPacket_Header * header = (WCPacket_Header*)buffer;
	DWORD written = 0;
	BOOL ret = WriteFile( handles[fd], buffer, header->length + sizeof(WCPacket_Header), &written, NULL);
	if( !ret )
		return -1;
	else
		return written;
}

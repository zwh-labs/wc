#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>

#include <stdio.h>
#include <malloc.h>



#ifdef _MSC_VER // http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
	#include <stdarg.h>
	#define snprintf c99_snprintf

	static int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
	{
		int count = -1;

		if (size != 0)
			count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
		if (count == -1)
			count = _vscprintf(format, ap);

		return count;
	}

	static int c99_snprintf(char* str, size_t size, const char* format, ...)
	{
		int count;
		va_list ap;

		va_start(ap, format);
		count = c99_vsnprintf(str, size, format, ap);
		va_end(ap);

		return count;
	}
#endif


int wcConfiguration_snprint( char * s, size_t n, const wcConfiguration * configuration )
{
	char * cur = s;	// points to current character in the buffer that would be written (might be past end of buffer)
	char * end = s + n;	// points to end of buffer

	char ** sp;	// points to the buffer for snprintf calls
	if( !s )	// if s is NULL pointer
		sp = &s;	// keep NULL pointer for every snprintf call
	else		// otherwise s is a pointer to a real buffer
		sp = &cur;	// point to the actual position in the buffer that should be written

	cur += snprintf( *sp, end-*sp, "Configuration %p:\n", (void*)configuration );
	cur += snprintf( *sp, end-*sp, "\tdevicePath=\"%s\"\n", wcConfiguration_getDevicePath( configuration ) );

	if( wcConfiguration_getWheelCount( configuration ) )
		cur += snprintf( *sp, end-*sp, "\tWheels:\n" );

	for( unsigned int i=0; i<wcConfiguration_getWheelCount( configuration ); i++ )
		cur += snprintf( *sp, end-*sp, "\t\t%u - %u incrementsPerTurn\n", i, wcConfiguration_getWheelIncrementsPerTurn( configuration, i ) );

	return cur-s;
}


int wcConfiguration_fprint( FILE * stream, const wcConfiguration * configuration )
{
	int size = wcConfiguration_snprint( NULL, 0, configuration );
	if( size < 0 )
		return -1;
	size += 1;	// + 1 for terminating '\0'
	char * buf = malloc( size );
	if( !buf )
		return -1;
	wcConfiguration_snprint( buf, size, configuration );
	int ret = fprintf( stream, "%s", buf );
	free( buf );
	return ret;
}


int wcConfiguration_print( const wcConfiguration * configuration )
{
	return wcConfiguration_fprint( stdout, configuration );
}

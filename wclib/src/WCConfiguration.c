#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define DEFAULT_WHEELINCREMENTSPERTURN (1024)
#define DEFAULT_WHEELDIAMETER           (1.0)

#ifdef _WIN32
	#define DEFAULT_DEVICEPATH "\\\\.\\COM1"
#else
	#define DEFAULT_DEVICEPATH "/dev/ttyACM0"
#endif


typedef struct WCConfigurationWheel
{
	unsigned int incrementsPerTurn;
	double wheelDiameter;
} WCConfigurationWheel;


typedef struct WCConfiguration
{
	char * devicePath;
	unsigned int wheelCount;
	WCConfigurationWheel * wheelConfigurations;
} WCConfiguration;


const char * WCConfiguration_getDevicePath( const WCConfiguration * configuration )
{
	return configuration->devicePath;
}


unsigned int WCConfiguration_getWheelCount( const WCConfiguration * configuration )
{
	return configuration->wheelCount;
}


unsigned int WCConfiguration_getWheelIncrementsPerTurn( const WCConfiguration * configuration, unsigned int wheelIndex )
{
	if( wheelIndex >= configuration->wheelCount )
		return DEFAULT_WHEELINCREMENTSPERTURN;
	return configuration->wheelConfigurations[wheelIndex].incrementsPerTurn;
}


double WCConfiguration_getWheelDiameter( const WCConfiguration * configuration, unsigned int wheelIndex )
{
	if( wheelIndex >= configuration->wheelCount )
		return DEFAULT_WHEELDIAMETER;
	return configuration->wheelConfigurations[wheelIndex].wheelDiameter;
}


void WCConfiguration_setDevicePath( WCConfiguration * configuration, const char * devicePath )
{
	free( configuration->devicePath );
	configuration->devicePath = malloc( strlen(devicePath) + 1 );
	strcpy( configuration->devicePath, devicePath );
}


int WCConfiguration_setWheel( WCConfiguration * configuration, unsigned int wheelIndex, unsigned int incrementsPerTurn, double wheelDiameter )
{
	if( wheelIndex >= configuration->wheelCount )
	{
		WCConfigurationWheel * newWheelConfigurations = realloc( configuration->wheelConfigurations, sizeof(WCConfigurationWheel) * (wheelIndex + 1) );
		if( !newWheelConfigurations )
			return -1;
		configuration->wheelConfigurations = newWheelConfigurations;
	}
	configuration->wheelCount = wheelIndex + 1;
	configuration->wheelConfigurations[wheelIndex].incrementsPerTurn = incrementsPerTurn;
	configuration->wheelConfigurations[wheelIndex].wheelDiameter = wheelDiameter;
	return 0;
}


void WCConfiguration_delete( WCConfiguration * configuration )
{
	free( configuration->devicePath );
	free( configuration->wheelConfigurations );
	free( configuration );
}


WCConfiguration * WCConfiguration_new()
{
	WCConfiguration * configuration = malloc( sizeof(WCConfiguration) );
	memset( configuration, 0, sizeof(WCConfiguration) );

	WCConfiguration_setDevicePath( configuration, DEFAULT_DEVICEPATH );
	configuration->wheelCount = 0;

	return configuration;
}


int WCConfiguration_fprint( FILE * stream, const WCConfiguration * configuration )
{
	int cnt = 0;
	cnt += fprintf( stream, "Configuration %p:\n", (void*)configuration );
	cnt += fprintf( stream, "\tdevicePath=\"%s\"\n", configuration->devicePath );
	cnt += fprintf( stream, "\tWheels:\n" );
	for( unsigned int i=0; i<configuration->wheelCount; i++ )
		cnt += fprintf( stream, "\t\t%u - %u incrementsPerTurn - %lf diameter\n", i, configuration->wheelConfigurations[i].incrementsPerTurn, configuration->wheelConfigurations[i].wheelDiameter );
	return cnt;
}

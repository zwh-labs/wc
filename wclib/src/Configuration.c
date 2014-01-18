#include <wc/Configuration.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define DEFAULT_WHEELINCREMENTSPERTURN (4096)
#define DEFAULT_WHEELDIAMETER           (1.0)

#ifdef _WIN32
	#define DEFAULT_DEVICEPATH "\\\\.\\COM1"
#else
	#define DEFAULT_DEVICEPATH "/dev/ttyACM0"
#endif


typedef struct _wcConfigurationWheel wcConfigurationWheel;
struct _wcConfigurationWheel
{
	unsigned int incrementsPerTurn;
};


struct _wcConfiguration
{
	char * devicePath;
	unsigned int wheelCount;
	wcConfigurationWheel * wheelConfigurations;
};


const char * wcConfiguration_getDevicePath( const wcConfiguration * configuration )
{
	return configuration->devicePath;
}


unsigned int wcConfiguration_getWheelCount( const wcConfiguration * configuration )
{
	return configuration->wheelCount;
}


unsigned int wcConfiguration_getWheelIncrementsPerTurn( const wcConfiguration * configuration, unsigned int wheelIndex )
{
	if( wheelIndex >= configuration->wheelCount )
		return DEFAULT_WHEELINCREMENTSPERTURN;
	return configuration->wheelConfigurations[wheelIndex].incrementsPerTurn;
}


void wcConfiguration_setDevicePath( wcConfiguration * configuration, const char * devicePath )
{
	free( configuration->devicePath );
	configuration->devicePath = (char*) malloc( strlen(devicePath) + 1 );
	strcpy( configuration->devicePath, devicePath );
}


bool wcConfiguration_setWheel( wcConfiguration * configuration, unsigned int wheelIndex, unsigned int incrementsPerTurn )
{
	if( wheelIndex >= configuration->wheelCount )
	{
		wcConfigurationWheel * newWheelConfigurations = (wcConfigurationWheel*) realloc( configuration->wheelConfigurations, sizeof(wcConfigurationWheel) * (wheelIndex + 1) );
		if( !newWheelConfigurations )
			return false;
		memset( newWheelConfigurations + configuration->wheelCount, 0, sizeof(wcConfigurationWheel) * (wheelIndex - configuration->wheelCount) );
		configuration->wheelConfigurations = newWheelConfigurations;
		configuration->wheelCount = wheelIndex + 1;
	}
	configuration->wheelConfigurations[wheelIndex].incrementsPerTurn = incrementsPerTurn;
	return true;
}


bool wcConfiguration_delete( wcConfiguration * configuration )
{
	if( !configuration )
		return false;
	free( configuration->devicePath );
	free( configuration->wheelConfigurations );
	free( configuration );
	return true;
}


wcConfiguration * wcConfiguration_new()
{
	wcConfiguration * configuration = (wcConfiguration*) malloc( sizeof(wcConfiguration) );
	memset( configuration, 0, sizeof(wcConfiguration) );

	wcConfiguration_setDevicePath( configuration, DEFAULT_DEVICEPATH );
	configuration->wheelCount = 0;

	return configuration;
}

#include <wc/WCConfiguration_ArgumentParser.h>
#include <wc/WCConfiguration.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//TODO: this needs improvement
WCConfiguration * WCConfiguration_newFromArguments( int argc, const char ** argv )
{
	WCConfiguration * configuration = WCConfiguration_new();
	for( int i=1; i<argc; i++ )
	{
		if( !strcmp( argv[i], "--devicePath" ) )
		{
			if( i < argc )
			{
				i++;
				WCConfiguration_setDevicePath( configuration, argv[i] );
			}
			continue;
		} else if( !strcmp( argv[i], "--wheel" ) )
		{
			if( i < argc )
			{
				i++;
				char * tmp = malloc( strlen( argv[i] ) + 1 );
				strcpy( tmp, argv[i] );

				unsigned int index;
				unsigned int incrementsPerTurn;
				char * token;
				token = strtok( tmp, ":" );
				if( token )
				{
					sscanf( token, "%u", &index );
				}
				token = strtok( NULL, ":" );
				if( token )
				{
					sscanf( token, "%u", &incrementsPerTurn );
				}
				WCConfiguration_setWheelIncrementsPerTurn( configuration, index, incrementsPerTurn );

				free( tmp );
			}
			continue;
		}
	}
	return configuration;
}

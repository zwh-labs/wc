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
				double diameter;
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
				token = strtok( NULL, ":" );
				if( token )
				{
					sscanf( token, "%lf", &diameter );
				}
				WCConfiguration_setWheel( configuration, index, incrementsPerTurn, diameter );

				free( tmp );
			}
			continue;
		}
	}
	WCConfiguration_fprint( stderr, configuration );
	return configuration;
}

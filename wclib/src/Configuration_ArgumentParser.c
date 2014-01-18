#include <wc/Configuration_ArgumentParser.h>
#include <wc/Configuration.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//TODO: this needs improvement
wcConfiguration * wcConfiguration_newFromArguments( int argc, const char ** argv )
{
	wcConfiguration * configuration = wcConfiguration_new();
	for( int i=1; i<argc; i++ )
	{
		if( !strcmp( argv[i], "--devicePath" ) )
		{
			if( i < argc )
			{
				i++;
				wcConfiguration_setDevicePath( configuration, argv[i] );
			}
			continue;
		} else if( !strcmp( argv[i], "--wheel" ) )
		{
			if( i < argc )
			{
				i++;
				char * tmp = (char*) malloc( strlen( argv[i] ) + 1 );
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
				wcConfiguration_setWheel( configuration, index, incrementsPerTurn );

				free( tmp );
			}
			continue;
		}
	}
	return configuration;
}

/* This example emulates wheels and sends the packets to a serial console.
 * Example usage:
 * - Create a pair of pseudo terminals using
 *     # socat -d -d pty,raw,echo=0 pty,raw,echo=0
 *   this will create a for example /dev/pts/4 and /dev/pts/5
 * - Run the emulator on one of the pseudo terminals
 *     # ./wcExampleSDL2OpenGLWCEmulator --devicePath /dev/pts/5
 * - Now connect another example to the other side
 *     # ./wcExampleSDL2OpenGLPID --devicePath /dev/pts/4
 */

#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>
#include <wc/Configuration_ArgumentParser.h>
#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>
#include <wc/WheelMovement.h>
#include <wc/WheelMovement_Configuration.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/glu.h>

#include <stdio.h>
#include <stdbool.h>
#include <math.h>


#define WHEELNUM 2


static SDL_Window * window = NULL;
static SDL_GLContext glContext = 0;

static wcConfiguration * configuration = NULL;
static wcConnection * connection = NULL;

static int wheelRotationIncrements[WHEELNUM] = { 0 };


bool init_sdl()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Failed to initialize SDL: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	window = SDL_CreateWindow( "WheelChar SDL2/OpenGL Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	if( !window )
	{
		fprintf( stderr, "Failed to create Window: %s\n", SDL_GetError() );
		return false;
	}

	glContext = SDL_GL_CreateContext( window );
	if( !glContext )
	{
		fprintf( stderr, "Failed to create OpenGL context: %s\n", SDL_GetError() );
		return false;
	}

	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		fprintf( stderr, "Unable to set swap interval: %s\n", SDL_GetError() );
	}

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	return true;
}


bool init_wc( int argc, const char ** argv )
{
	configuration = wcConfiguration_newFromArguments( argc, argv );
	if( !configuration )
	{
		fprintf( stderr, "Failed to read configuration\n" );
		return false;
	}
	wcConfiguration_fprint( stderr, configuration );

	connection = wcConnection_openFromConfiguration( configuration );
	if( !connection )
	{
		fprintf( stderr, "Failed to open connection\n" );
		return false;
	}

	return true;
}


bool init( int argc, const char ** argv )
{
	if( !init_wc( argc, argv ) )
		return false;

	if( !init_sdl() )
		return false;

	return true;
}


static inline double fraction( double d )
{
	return d - floor(d);
}


static int mouseX = 0;
static int mouseY = 0;


void emulatedWheelMovement_update()
{
	SDL_GetRelativeMouseState( &mouseX, &mouseY );
}


int emulatedWheelMovement_getIncrements( unsigned int i )
{
	int increments = 0.0;
	i++;
	if( i&1 )
		increments += mouseX;
	if( i&2 )
		increments += mouseY * 8;
	return increments;
}


void sendWheelMovement( unsigned int index, int movementIncrements )
{
	wcPacket packet;
	wcPacket_Wheel_create( (wcPacket_Wheel*)&packet, index, WC_ERROR_NOERROR, movementIncrements );
	wcConnection_write( connection, &packet );
}


// http://stackoverflow.com/questions/12089514/real-modulo-operator-in-c-c
static inline int modulo( int a, int b )
{
	const int result = a % b;
	return result >= 0 ? result : result + b;
}


void update()
{
	glClear( GL_COLOR_BUFFER_BIT );

	emulatedWheelMovement_update();

	for( unsigned int i=0; i<WHEELNUM; i++ )
	{
		int increments = emulatedWheelMovement_getIncrements( i );
		sendWheelMovement( i, increments );
		wheelRotationIncrements[i] += increments;
		wheelRotationIncrements[i] = modulo( wheelRotationIncrements[i], wcConfiguration_getWheelIncrementsPerTurn( configuration, i ) );
		glPushMatrix();
			unsigned char color = i+1;
			glColor3f( color&1, color&2, color&4 );
			glRotatef( ( (double)wheelRotationIncrements[i] / (double)wcConfiguration_getWheelIncrementsPerTurn( configuration, i ) ) * 360.0f, 0,0,1 );
			glBegin( GL_QUADS );
				glVertex2f( -0.5f, -0.5f );
				glVertex2f( 0.5f, -0.5f );
				glVertex2f( 0.5f, 0.5f );
				glVertex2f( -0.5f, 0.5f );
			glEnd();
		glPopMatrix();
	}
}


void quit_sdl()
{
	SDL_GL_DeleteContext( glContext );
	glContext = 0;
	SDL_DestroyWindow( window );
	window = NULL;
	SDL_Quit();
}


void quit_wc()
{
	wcConnection_close( connection );
	connection = NULL;
	wcConfiguration_delete( configuration );
	configuration = NULL;
}


void quit()
{
	quit_sdl();
	quit_wc();
}


int main( int argc, const char ** argv )
{
	if( !init( argc, argv ) )
		return 1;

	bool run = true;
	while( run )
	{
		SDL_Event e;
		while( SDL_PollEvent( &e ) )
		{
			switch( e.type )
			{
			case SDL_QUIT:
				run = false;
				break;
			case SDL_WINDOWEVENT:
				switch( e.window.event )
				{
				case SDL_WINDOWEVENT_RESIZED:
					glViewport( 0, 0, e.window.data1, e.window.data2 );
					break;
				}
				break;
			}
		}
		update();
		SDL_GL_SwapWindow( window );
	}

	quit();
	return 0;
}

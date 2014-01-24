#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>
#include <wc/Configuration_ArgumentParser.h>
#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>
#include <wc/Thread.h>
#include <wc/WheelMovement.h>
#include <wc/WheelMovement_Configuration.h>
#include <wc/WheelPIDController.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/glu.h>

#include <stdio.h>
#include <stdbool.h>


#define WHEELNUM 2


static SDL_Window * window = NULL;
static SDL_GLContext glContext = 0;

static wcConfiguration * configuration = NULL;
static wcConnection * connection = NULL;
static wcThread * thread = NULL;

static wcWheelPIDController * wheelControllers[WHEELNUM] = { 0 };
static int targetWheelAngleIncrements[WHEELNUM] = { 0 };
static double wheelRotations[WHEELNUM] = { 0.0f };
static double wheelVelocities[WHEELNUM] = { 0.0f };


bool init_sdl()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Failed to initialize SDL: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	window = SDL_CreateWindow( "WheelChar SDL2/OpenGL Example PID Control", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
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

	thread = wcThread_start( connection );
	if( !thread )
	{
		fprintf( stderr, "Failed to start thread\n" );
		return false;
	}

	for( int i=0; i<WHEELNUM; i++ )
	{
		wheelControllers[i] = wcWheelPIDController_new( configuration, i );
		if( !wheelControllers[i] )
		{
			fprintf( stderr, "Failed to instantiate PID controller\n" );
			return false;
		}
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


void update()
{
	static unsigned int lastTime = 0;
	static unsigned int currentTime = 0;

	currentTime = SDL_GetTicks();
	double delta = (double)(currentTime - lastTime) / 1000.0;
	lastTime = currentTime;

	glClear( GL_COLOR_BUFFER_BIT );

	for( int i=0; i<WHEELNUM; i++ )
	{
		int incrementsPerTurn = wcConfiguration_getWheelIncrementsPerTurn( configuration, i );

		wcWheelMovement wm = wcThread_retrieveWheelMovement( thread, i );
		targetWheelAngleIncrements[i] += wcWheelMovement_getIncrements( &wm );
		targetWheelAngleIncrements[i] %= incrementsPerTurn;

		int actualAngleIncrements = wheelRotations[i] * (double)incrementsPerTurn;
		double controlValue = wcWheelPIDController_update( wheelControllers[i], targetWheelAngleIncrements[i], actualAngleIncrements, delta );
		wheelVelocities[i] += controlValue;
		wheelRotations[i] += wheelVelocities[i];
		wheelRotations[i] = fraction(wheelRotations[i]);
		glPushMatrix();
			unsigned char color = i+1;
			glColor3f( color&1, color&2, color&4 );
			glRotatef( wheelRotations[i] * 360.0f, 0,0,1 );
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
	wcThread_stop( thread );
	thread = NULL;
	wcConnection_close( connection );
	connection = NULL;
	wcConfiguration_delete( configuration );
	configuration = NULL;
	for( int i=0; i<WHEELNUM; i++ )
		wcWheelPIDController_delete( wheelControllers[i] );
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

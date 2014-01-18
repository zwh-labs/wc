#include <wc/Configuration.h>
#include <wc/Configuration_stdio.h>
#include <wc/Configuration_ArgumentParser.h>
#include <wc/Connection.h>
#include <wc/Connection_Configuration.h>
#include <wc/Thread.h>
#include <wc/WheelMovement.h>
#include <wc/WheelMovement_Configuration.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <GL/glu.h>

#include <stdio.h>
#include <stdbool.h>


SDL_Window * window = NULL;
SDL_GLContext glContext = 0;

wcConfiguration * configuration = NULL;
wcConnection * connection = NULL;
wcThread * thread = NULL;


bool init_sdl()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Failed to initialize SDL: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

	window = SDL_CreateWindow( "WheelChar SDL2/OpenGL Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
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


void update()
{
	glClear( GL_COLOR_BUFFER_BIT );

	wcWheelMovement wmA = wcThread_retrieveWheelMovement( thread, 0 );
	wcWheelMovement wmB = wcThread_retrieveWheelMovement( thread, 1 );

	static float rotA = 0;
	static float rotB = 0;
	rotA += wcWheelMovement_getTurns( &wmA, configuration ) * 360.0f;
	rotB += wcWheelMovement_getTurns( &wmB, configuration ) * 360.0f;

	glPushMatrix();
		glColor3f(1,0,0);
		glRotatef( rotB, 0,0,1 );
		glBegin( GL_QUADS );
			glVertex2f( -0.5f, -0.5f );
			glVertex2f( 0.5f, -0.5f );
			glVertex2f( 0.5f, 0.5f );
			glVertex2f( -0.5f, 0.5f );
		glEnd();
	glPopMatrix();
	glPushMatrix();
		glColor3f(0,1,0);
		glRotatef( rotA, 0,0,1 );
		glBegin( GL_QUADS );
			glVertex2f( -0.5f, -0.5f );
			glVertex2f( 0.5f, -0.5f );
			glVertex2f( 0.5f, 0.5f );
			glVertex2f( -0.5f, 0.5f );
		glEnd();
	glPopMatrix();
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

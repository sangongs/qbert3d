#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <exception>

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

class SDLLayer
{
private:
	SDL_Surface *_screen;

public:

	unsigned Width, Height;

	SDLLayer(unsigned width, unsigned height, unsigned depth) : Width(width), Height(height)
	{
		if (SDL_Init(SDL_INIT_VIDEO)) 
			throw std::exception("Call to SDL_INIT_VIDEO failed...");

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

		_screen = SDL_SetVideoMode(width, height, depth, SDL_OPENGL | SDL_RESIZABLE);
		if (!_screen)
			throw std::exception("Call to SDL_SetVideoMode failed...");

	}

	~SDLLayer()
	{
		if (_screen)
			SDL_FreeSurface(_screen);

		SDL_Quit();
	}
};

int main(int argc, char *argv[])
{
	SDLLayer window(600,400,8);

	glShadeModel(GL_SMOOTH);

	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //[todo] understand what this does.

	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClearDepth(1.0f);
	

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, window.Width, window.Height ); //[todo] understand what this does.
	gluPerspective(45.0f, (GLfloat)window.Width / (GLfloat)window.Height, 0.1f, 100.0f);
	//glRotatef(10.0f, 0.0f , 1.0f, 0.0f);
	
	
	for (GLfloat alpha = 0; alpha < 1000; alpha ++)
	{

	
		glMatrixMode( GL_MODELVIEW );
		
		glLoadIdentity();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		glTranslatef(0.0f, 0.0f, -5.0f);
		glRotatef(alpha, 1, 0 , 0);

		glColor3f(0.5f,0.5f,1.0f);

		glBegin(GL_QUADS);					// Start Drawing Quads
		glVertex3f(-1.0f, -1.0f, -1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(1.0f, -1.0f, -1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(-1.0f, 1.0f, -1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(1.0f, 1.0f, -1.0f);			// Left And Down One Unit (Bottom Left)


		glColor3f(0.1f,0.67f,1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(1.0f, -1.0f, 1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(1.0f, 1.0f, 1.0f);			// Left And Down One Unit (Bottom Left)

		glColor3f(0.7f,1.0f,1.0f);	
		glVertex3f(1.0f, -1.0f, -1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(1.0f, -1.0f, 1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(1.0f, 1.0f, -1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(1.0f, 1.0f, 1.0f);			// Left And Down One Unit (Bottom Left)


		glColor3f(0.5f,0.5f,0.5f);
		glVertex3f(-1.0f, -1.0f, -1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(-1.0f, -1.0f, 1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(-1.0f, 1.0f, -1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Left And Down One Unit (Bottom Left)

		glColor3f(0.2f,0.1f,1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(1.0f, 1.0f, -1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(1.0f, 1.0f, 1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Left And Down One Unit (Bottom Left)

		glColor3f(1.0f, 1.0f ,1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);			// Left And Up 1 Unit (Top Left)
		glVertex3f(1.0f, -1.0f, -1.0f);			// Right And Up 1 Unit (Top Right)
		glVertex3f(1.0f, -1.0f, 1.0f);			// Right And Down One Unit (Bottom Right)
		glVertex3f(-1.0f, -1.0f, 1.0f);			// Left And Down One Unit (Bottom Left)

		glEnd();	

		SDL_GL_SwapBuffers();
		//SDL_Delay(2);
	}

	SDL_Delay(2000);

	return 1;
}
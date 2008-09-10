#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "MathUtils.h"

#include "FontView.h"

namespace BGComplete
{

FontView::FontView(const std::string& fileName, unsigned fontSize) : _texture(0)
{
	if (TTF_Init())
		throw std::exception("Can't intilize SDL_TTF");

	_font = TTF_OpenFont(fileName.c_str(), fontSize);
	if (!_font)
		throw std::exception("Error loading font");
}

FontView::~FontView()
{
	if (_texture)
	{
		glFinish();
		glDeleteTextures(1, &_texture);
	}
	TTF_CloseFont(_font);
}

void FontView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/){}
void FontView::Init(){}

void FontView::SetText(const std::string& text, unsigned char r, unsigned char g, unsigned char b, float* points)
{
	if (_texture)
	{
		glFinish();
		glDeleteTextures(1, &_texture);
	}

	memcpy(_quadPoints, points, sizeof(float) * 12);
	SDL_Color color = {r, g, b, 0};

	SDL_Surface *firstText = TTF_RenderText_Blended(_font, text.c_str(), color );
	SDL_Surface *intermediary = SDL_CreateRGBSurface(0, Math::NextPowerOfTwo(firstText->w), Math::NextPowerOfTwo(firstText->h), 32, 
		0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_BlitSurface(firstText, 0, intermediary, 0);

	glGenTextures( 1, &_texture );
	glBindTexture( GL_TEXTURE_2D, _texture);
	glTexImage2D( GL_TEXTURE_2D, 0, 4, intermediary->w, intermediary->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, intermediary->pixels );
	SDL_FreeSurface(firstText);
	SDL_FreeSurface(intermediary);
}

void FontView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
{
	if (clearAndSwap)
	{
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glViewport(startX, startY, width, height);
	glBindTexture( GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);

	glDisable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(_quadPoints);
	glTexCoord2f(1.0f, 0.0f);
		glVertex3fv(_quadPoints + 3);
	glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(_quadPoints + 6);
	glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(_quadPoints + 9);
	glEnd();
	
	glDisable(GL_BLEND);

	if (clearAndSwap)
		SDL_GL_SwapBuffers();
}

}
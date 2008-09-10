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

	_font = FontEncapsulator_Ptr(new FontEncapsulator(fileName, fontSize));
}

FontView::~FontView()
{
	if (_texture)
	{
		glFinish();
		glDeleteTextures(1, &_texture);
	}
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

	if (points == 0)
		_fitView = true;
	else
	{
		memcpy(_quadPoints, points, sizeof(float) * 12);
		_fitView = false;
	}

	SDL_Color color = {r, g, b, 0};

	SDL_Surface *firstText = TTF_RenderText_Blended(_font->Font, text.c_str(), color );
	_clip.Points[0] = (float)firstText->w / (float)Math::NextPowerOfTwo(firstText->w);
	_clip.Points[1] = (float)firstText->h / (float)Math::NextPowerOfTwo(firstText->h);
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
	glFrustum(-(float)width / 2.0f, (float)width / 2.0f, -(float)height / 2.0f, (float)height / 2.0f, 1.0f, 1000.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);

	if (_fitView)
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-(float)width * 0.5f, (float)height * 0.5f, -1.0f);
		glTexCoord2f(_clip.Points[0], 0.0f);
		glVertex3f((float)width * 0.5f, (float)height * 0.5f, -1.0f);
		glTexCoord2f(_clip.Points[0], _clip.Points[1]);
		glVertex3f((float)width * 0.5f, -(float)height * 0.5f, -1.0f);
		glTexCoord2f(0.0f, _clip.Points[1]);
		glVertex3f(-(float)width * 0.5f, -(float)height * 0.5f, -1.0f);
	}
	else
	{
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(_quadPoints);
		glTexCoord2f(_clip.Points[0], 0.0f);
		glVertex3fv(_quadPoints + 3);
		glTexCoord2f(_clip.Points[0], _clip.Points[1]);
		glVertex3fv(_quadPoints + 6);
		glTexCoord2f(0.0f, _clip.Points[1]);
		glVertex3fv(_quadPoints + 9);
	}
	
	glEnd();
	
	glDisable(GL_BLEND);

	if (clearAndSwap)
		SDL_GL_SwapBuffers();
}

}
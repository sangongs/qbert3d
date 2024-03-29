#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "FPSHud.h"

namespace BGComplete
{

FPSHud::FPSHud(const std::string& font, unsigned fontSize) : _drawer(font, fontSize)
{}

void FPSHud::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/){}

void FPSHud::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
{
	_frameAccumulator++;
	DWORD newTime = GetTickCount();
	_timeAccumulator += newTime - _lastTime;
	_lastTime = newTime;

	float points[] = 
	{-(float)width / 2.0f, (float)height / 2.0f, -1.0f,
	(float)width / 2.0f, (float)height / 2.0f, -1.0f,
	(float)width / 2.0f, -(float)height / 2.0f, -1.0f,
	-(float)width / 2.0f, -(float)height / 2.0f, -1.0f};

	if (_timeAccumulator > 1000)
	{
		_drawer.SetText((boost::format("%1% FPS") % (_frameAccumulator / (_timeAccumulator / 1000))).str().c_str(), 255, 255, 255, points);
		_timeAccumulator = _frameAccumulator = 0;
	}

	_drawer.Draw(clearAndSwap, startX, startY, width, height);
}

void FPSHud::Init(){}

}
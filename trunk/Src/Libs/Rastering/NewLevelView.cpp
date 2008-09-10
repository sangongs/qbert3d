#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "NewLevelView.h"

namespace BGComplete
{
	NewLevelView::NewLevelView() : _newLevelFont("FONTS\\CALIBRI.TTF", 28), _lastLevelTime(0), _lastLevel(0)
	{}

	void NewLevelView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	void NewLevelView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (GetTickCount() - _lastLevelTime < 1500)
			_newLevelFont.Draw(clearAndSwap, startX, startY, width, height);
	}

	void NewLevelView::Init() {}

	void NewLevelView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		if (modelObjects->Level > _lastLevel)
		{
			_lastLevelTime = GetTickCount();
			_lastLevel = modelObjects->Level;
			_newLevelFont.SetText((boost::format("Level %1%") % _lastLevel).str().c_str(), 255, 255, 255, 0);
		}
	}
}

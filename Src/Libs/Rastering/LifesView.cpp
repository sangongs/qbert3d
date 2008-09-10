#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "LifesView.h"

namespace BGComplete
{
	LifesView::LifesView() : _lifesFont("FONTS\\CALIBRI.TTF", 28), _currentLifes(-1)
	{}

	void LifesView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	void LifesView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		_lifesFont.Draw(clearAndSwap, startX, startY, width, height);
	}

	void LifesView::Init() {}

	void LifesView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		if (modelObjects->LivesLeft != _currentLifes) //this saves font rendering time
		{
			_currentLifes = modelObjects->LivesLeft;
			_lifesFont.SetText((boost::format("Lifes: %1%") % _currentLifes).str().c_str(), 255, 255, 255, 0);
		}
	}
}

#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "ScoreView.h"

namespace BGComplete
{
	ScoreView::ScoreView() : _font("FONTS\\CALIBRI.TTF", 28)
	{}

	void ScoreView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	void ScoreView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		float points[] = {
			-(float)width * 0.5f, (float)height * 0.5f, -3.0f,
			(float)width * 0.5f, (float)height * 0.5f, -3.0f,
			(float)width * 0.5f, -(float)height * 0.5f, -1.0f,
			-(float)width * 0.5f, -(float)height * 0.5f, -1.0f};

		_font.SetText((boost::format("%1%") % _modelObjects->Score).str().c_str(), 255, 255, 255, points);
		_font.Draw(clearAndSwap, startX, startY, width, height);
	}

	void ScoreView::Init() {}
	void ScoreView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		_modelObjects = modelObjects;
	}
}

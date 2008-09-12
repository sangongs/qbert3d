#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"
#include "MathUtils.h"

#include "GameStartView.h"

namespace BGComplete
{
	GameStartView::GameStartView() : _gameStartFont("FONTS\\CALIBRI.TTF", 28), _pressNFont("FONTS\\CALIBRI.TTF", 28), _bgComplete("FONTS\\CALIBRI.TTF", 28)
	{}

	void GameStartView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	float swing()
	{
		float stage = (float)(GetTickCount() % 1000);
		if (stage > 500)
			stage = 1000 - stage;
		return stage / 500;
	}

	void GameStartView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (_modelObjects->gameStage == DidntStart)
		{
			_gameStartFont.Draw(clearAndSwap, startX, (startY + (unsigned)(height * 0.5)), width, unsigned(height * 0.5));
			_bgComplete.Draw(clearAndSwap, startX + (unsigned)(width * 0.4), startY + (unsigned)(height * 0.4), (unsigned)(width * 0.6), unsigned(height * 0.1));

			if (Math::Swing(1000) > 0.5)
				_pressNFont.Draw(clearAndSwap, startX, startY, width, (unsigned)((float)height * 0.4f));
		}
	}

	void GameStartView::Init() 
	{
		_gameStartFont.SetText("Qbert 3D", 255, 255, 255, 0);
		_bgComplete.SetText("By BGComplete", 255, 255, 255, 0); //Name of our company right here!
		_pressNFont.SetText("Press N to start game", 255, 255, 255, 0);
	}
	void GameStartView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		_modelObjects = modelObjects;
	}
}

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
		if (_modelObjects->gameStage == GameStage::DidntStart)
		{
			_gameStartFont.Draw(clearAndSwap, startX, (unsigned)(startY + (float)height * 0.5f), width, unsigned((float)height * 0.5f));
			_bgComplete.Draw(clearAndSwap, startX + (float)width * 0.4f, (unsigned)(startY + (float)height * 0.4f), (float)width * 0.6f, unsigned((float)height * 0.1f));

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

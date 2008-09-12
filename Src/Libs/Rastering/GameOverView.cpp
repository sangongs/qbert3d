#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"
#include "MathUtils.h"

#include "GameOverView.h"

namespace BGComplete
{
	GameOverView::GameOverView() : _gameOverFont("FONTS\\CALIBRI.TTF", 28), _pressNFont("FONTS\\CALIBRI.TTF", 28)
	{
	}

	void GameOverView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	void GameOverView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if ((_modelObjects->gameStage == GameStage::GameOver) || (_modelObjects->gameStage == GameStage::GameWon))
		{
			_gameOverFont.SetText(_modelObjects->gameStage == GameStage::GameWon ? "Congratulations you finished the game!" : "Game Over!", 255, 255, 255, 0);
			_gameOverFont.Draw(clearAndSwap, startX, (unsigned)(startY + (float)height * 0.4f), width, unsigned((float)height * 0.6f));
			
			if (Math::Swing(1000) > 0.5)
			_pressNFont.Draw(clearAndSwap, startX, startY, width, (unsigned)((float)height * 0.4f));
		}
	}

	void GameOverView::Init() 
	{
		
		_pressNFont.SetText("Press N to restart game", 255, 255, 255, 0);
	}
	void GameOverView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		_modelObjects = modelObjects;
	}
}

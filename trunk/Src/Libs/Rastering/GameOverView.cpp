#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "GameOverView.h"

namespace BGComplete
{
	GameOverView::GameOverView() : _gameOverFont("FONTS\\CALIBRI.TTF", 28), _pressNFont("FONTS\\CALIBRI.TTF", 28)
	{
	}

	void GameOverView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	float swing()
	{
		float stage = (GetTickCount() % 1000);
		if (stage > 500)
			stage = 1000 - stage;
		return stage / 500;
	}

	void GameOverView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		//if (_modelObjects->gameStage == GameStage::GameOver)
		if (true)
		{
			float points[] = {
				-(float)width * 0.5f, (float)height * 0.3f, -1.0f,
				(float)width * 0.5f, (float)height * 0.3f, -1.0f,
				(float)width * 0.5f, -(float)height * 0.3f, -1.0f,
				-(float)width * 0.5f, -(float)height * 0.3f, -1.0f};

			_gameOverFont.SetText("Game Over!", 255, 255, 255, points);
			_gameOverFont.Draw(clearAndSwap, startX, startY + (float)height * 0.4f, width, (float)height * 0.6f);
			
			if (swing() > 0.5)
			_pressNFont.Draw(clearAndSwap, startX, startY, width, (float)height * 0.4f);
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

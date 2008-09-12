#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Point3D.h"

#include "QbertModel.h"
#include "ArielView.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"
#include "QbertGameModel.h"

#include "SimpleControler.h"

namespace BGComplete
{
	class SDLLayer
	{
	private:
		SDL_Surface *_screen;

	public:

		unsigned Width, Height;

		SDLLayer(unsigned width, unsigned height, unsigned depth) : Width(width), Height(height)
		{
			if (SDL_Init(SDL_INIT_VIDEO)) 
				throw std::exception("Call to SDL_INIT_VIDEO failed...");

			SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

			_screen = SDL_SetVideoMode(width, height, depth, SDL_OPENGL | SDL_RESIZABLE);
			if (!_screen)
				throw std::exception("Call to SDL_SetVideoMode failed...");

			SDL_WM_SetCaption("QBert 3D by BGComplete", 0);
		}

		~SDLLayer()
		{
			if (_screen)
				SDL_FreeSurface(_screen);

			SDL_Quit();
		}
	};


	SimpleControler::SimpleControler(QbertView * view, QbertGameModel * model) : _view(view), _model(model), _leftMouseKeyDown(false), _running(true)
	{
	}

	void SimpleControler::ReadInput(DWORD deltaTime)
	{
		if (_leftMouseKeyDown)
		{
			int newX, newY;
			SDL_GetRelativeMouseState(&newX, &newY);
			_view->CameraMove(0, 0, 0, 0 , (float)newX / 4.0f , 0, 0);
			_view->CameraMove(0, 0, 0, (float)newY / 4.0f, 0 , 0, 0);
		}

		InputData inputData(deltaTime);

		SDL_Event event;
		while(SDL_PollEvent(&event)) 
		{
			switch(event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				if( event.button.button == SDL_BUTTON_LEFT ) 
				{
					_leftMouseKeyDown = true;
					SDL_GetRelativeMouseState(int(), int());
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if( event.button.button == SDL_BUTTON_LEFT ) 
					_leftMouseKeyDown = false;
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_KP_PLUS)
					_view->CameraMove(0, 0, -1, 0, 0, 0, 0);
				else if (event.key.keysym.sym == SDLK_KP_ENTER)
					_view->CameraMove(0, 0, 1, 0, 0, 0, 0);
				else if (event.key.keysym.sym == SDLK_UP)
					inputData.direction = Up;
				else if (event.key.keysym.sym == SDLK_DOWN)
					inputData.direction = Down;
				else if (event.key.keysym.sym == SDLK_LEFT)
					inputData.direction = Left;
				else if (event.key.keysym.sym == SDLK_n)
					inputData.StartNewGame = true;
				else if (event.key.keysym.sym == SDLK_RIGHT)
					inputData.direction = Right;
				else if (event.key.keysym.sym == SDLK_v)
					_view->CameraMove(0, 0, 0, 0, 0, 0, 'v');
				if (event.key.keysym.sym == SDLK_SLASH)
					_view->CameraMove(0, 0, 0, 0, 0, 0, '/');
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SLASH)
					_view->CameraMove(0, 0, 0, 0, 0, 0, '/');
				break;
			case SDL_VIDEORESIZE:
				_width = event.resize.w;
				_height = event.resize.h;
				break;
			case SDL_QUIT:
				_running = false;
				return;
			}
		}

		_model->ReciveInput(inputData);
	}

	void SimpleControler::Run(unsigned startWidth, unsigned startHeight, unsigned depth)
	{
		SDLLayer sdl(startWidth, startHeight, depth);
		_view->Init();
		_width = startWidth;
		_height = startHeight;
		

		DWORD lastTime = GetTickCount();

		while(_running)
		{
			DWORD tempTime = GetTickCount();
			ReadInput(tempTime - lastTime);
			lastTime = tempTime;
			
			_view->SetUpDrawModel(_model->GetModelObjects());
			_view->Draw(true, 0, 0, _width, _height);

			SDL_Delay(20);
		}
	}
}

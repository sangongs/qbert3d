#include "StdAfx.h"

#include "View.h"
#include "Model.h"

#include "SimpleControler.h"

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

	}

	~SDLLayer()
	{
		if (_screen)
			SDL_FreeSurface(_screen);

		SDL_Quit();
	}
};


SimpleControler::SimpleControler)(View * view, Model * model) : _view(view), _model(_model), _leftMouseKeyDown(false), _running(true)
{
}

void SimpleControler::Input()
{
	if (_leftMouseKeyDown)
	{
		int newX, newY;
		SDL_GetRelativeMouseState(&newX, &newY);
		_view->CameraMove(0, 0, 0, newX, 0, 0);
		_view->CameraMove(0, 0, 0, 0, newY, 0);
	}

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
		case SDL_KEYDOWN:
			break;
		case SDL_KEYUP:
			// 					DisplayState(&event.key);
			// 					DisplayModifiers(&event.key);
			// 					DisplayKey(&event.key);
			break;
		case SDL_QUIT:
			running = false;
			return;
			break;
		}
	}
}

void SimpleControler::Run(unsigned startWidth, unsigned startHeight, unsigned depth)
{
	SDLLayer sdl(startWidth, startHeight, depth);
	_view->ChangeResolution(startWidth, startHeight);
	
	bool running(true);


	DWORD lastTime = GetTickCount();

	while(_running)
	{
		Input();
		_view->Init(startWidth, startHeight);

		DWORD tempTime = GetTickCount();
		_model->TimeTick(lastTime - tempTime);
		lastTime = tempTime;

		_view->SetGameObjects(_model->GetObjects());
		_view->Draw();

		SDL_Delay(2); //[todo] maybe we don't need this
	}
}

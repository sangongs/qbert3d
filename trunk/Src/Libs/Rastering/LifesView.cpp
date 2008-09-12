#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "LifesView.h"

namespace BGComplete
{
	LifesView::LifesView() : _currentLifes(-1)
	{}

	void LifesView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/) {}

	void LifesView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		glViewport(startX, startY, width, height );
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //[todo] understand what this does.
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho(-0.5f, 0.5f, -(float)height / (float)width * 0.5f, (float)height / (float)width * 0.5f, 1.0f, 1000.0f);

		if (clearAndSwap)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float lightParams[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f, 0.2f, 0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f};
		glLightfv(GL_LIGHT0, GL_POSITION, lightParams);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightParams + 4);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightParams + 7);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightParams + 10);
		glLoadIdentity();

		glTranslatef((1.0f / 6.0f) * - 2.5, 0, -5.0f);
		for (int i = 0; i < _currentLifes; i++)
		{
			_heart->Draw(0, 0, 0, 0.16);
			glTranslatef((1.0f / 6.0f), 0, 0);
		}

		if (clearAndSwap)
			SDL_GL_SwapBuffers();
	}

	void LifesView::Init() 
	{
		 _heart = DrawableObj_Ptr(new DrawableObj("Objects", "3D_heart.obj", 1, 0, 0, 0));
	}

	void LifesView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		_currentLifes = modelObjects->LivesLeft;
	}
}

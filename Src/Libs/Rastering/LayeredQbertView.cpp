#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Math.h"
#include "Point3D.h"

#include "LayeredQbertView.h"

namespace BGComplete
{

	LayeredQbertView::LayeredQbertView(std::list<QbertView_Ptr>& inp) : _views(inp)
	{}

	void LayeredQbertView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey)
	{
		BOOST_FOREACH(QbertView_Ptr iter, _views)
			iter->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, viewKey);
	}

	void LayeredQbertView::Draw(QbertModel::ModelObjects& modelObjects, bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (clearAndSwap)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		BOOST_FOREACH(QbertView_Ptr iter, _views)
		{
			iter->Draw(modelObjects, false, startX, startY, width, height);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		if (clearAndSwap)
			SDL_GL_SwapBuffers();
	}

	void LayeredQbertView::Init()
	{
		BOOST_FOREACH(QbertView_Ptr iter, _views)
			iter->Init();
	}

}

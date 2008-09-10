#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "MathUtils.h"
#include "Point3D.h"

#include "LayeredQbertView.h"

namespace BGComplete
{

	LayeredQbertView::LayeredQbertView(std::list<QbertView_Ptr>& qbertViews, std::list<View_Ptr>& simpleViews) : _qbertViews(qbertViews) , _simpleViews(simpleViews)
	{}

	void LayeredQbertView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey)
	{
		BOOST_FOREACH(QbertView_Ptr iter, _qbertViews)
			iter->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, viewKey);
		BOOST_FOREACH(View_Ptr iter, _simpleViews)
			iter->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, viewKey);
	}

	void LayeredQbertView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		BOOST_FOREACH(QbertView_Ptr iter, _qbertViews)
			iter->SetUpDrawModel(modelObjects);
	}

	void LayeredQbertView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (clearAndSwap)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		BOOST_FOREACH(QbertView_Ptr iter, _qbertViews)
		{
			iter->Draw(false, startX, startY, width, height);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		
		BOOST_FOREACH(View_Ptr iter, _simpleViews)
		{
			iter->Draw(false, startX, startY, width, height);
			glClear(GL_DEPTH_BUFFER_BIT);	
		}

		if (clearAndSwap)
			SDL_GL_SwapBuffers();
	}

	void LayeredQbertView::Init()
	{
		BOOST_FOREACH(QbertView_Ptr iter, _qbertViews)
			iter->Init();
		BOOST_FOREACH(View_Ptr iter, _simpleViews)
			iter->Init();
	}

}

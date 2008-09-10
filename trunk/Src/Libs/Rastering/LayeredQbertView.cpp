#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "MathUtils.h"
#include "Point3D.h"

#include "LayeredQbertView.h"

namespace BGComplete
{

	LayeredQbertView::LayeredQbertView(std::list<CoordinatedQbertView>& qbertViews, std::list<CoordinatedSimpleView>& simpleViews) : _qbertViews(qbertViews) , _simpleViews(simpleViews)
	{}

	void LayeredQbertView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey)
	{
		BOOST_FOREACH(CoordinatedQbertView iter, _qbertViews)
			iter.first->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, viewKey);
		BOOST_FOREACH(CoordinatedSimpleView iter, _simpleViews)
			iter.first->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, viewKey);
	}

	void LayeredQbertView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
	{
		BOOST_FOREACH(CoordinatedQbertView iter, _qbertViews)
			iter.first->SetUpDrawModel(modelObjects);
	}

	void LayeredQbertView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (clearAndSwap)
		{
			glClearDepth(1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		BOOST_FOREACH(CoordinatedQbertView iter, _qbertViews)
		{
			iter.first->Draw(false,
				(unsigned)((float)startX + width * iter.second.first.Points[0]),
				(unsigned)((float)startY + height * iter.second.first.Points[1]),
				(unsigned)((float)startX + width * iter.second.second.Points[0]),
				(unsigned)((float)startY + height * iter.second.second.Points[1]));
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		
		BOOST_FOREACH(CoordinatedSimpleView iter, _simpleViews)
		{
			iter.first->Draw(false,
				(unsigned)((float)startX + width * iter.second.first.Points[0]),
				(unsigned)((float)startY + height * iter.second.first.Points[1]),
				(unsigned)((float)startX + width * iter.second.second.Points[0]),
				(unsigned)((float)startY + height * iter.second.second.Points[1]));
			glClear(GL_DEPTH_BUFFER_BIT);	
		}

		if (clearAndSwap)
			SDL_GL_SwapBuffers();
	}

	void LayeredQbertView::Init()
	{
		BOOST_FOREACH(CoordinatedQbertView iter, _qbertViews)
			iter.first->Init();
		BOOST_FOREACH(CoordinatedSimpleView iter, _simpleViews)
			iter.first->Init();
	}

}

#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Math.h"
#include "Point3D.h"

#include "DoubleQbertView.h"

namespace BGComplete
{
	
DoubleQbertView::DoubleQbertView(QbertView_Ptr view1, QbertView_Ptr view2, float ratio) : _ratio(ratio), _state(0)
{
	_views[0] = view1;
	_views[1] = view2;
}

void DoubleQbertView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, bool viewChanged)
{
	_state ^= viewChanged;
	_views[0]->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, false);
	_views[1]->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, false);
}

void DoubleQbertView::Draw(QbertModel::ModelObjects& modelObjects, bool clear, unsigned startX, unsigned startY, unsigned width, unsigned height)
{
	if (clear)
	{
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	_views[_state]->Draw(modelObjects, false, startX, startY, (unsigned)(width * (1 - _ratio)), (unsigned)(height * (1 - _ratio)));
	_views[!_state]->Draw(modelObjects, false, 
		(unsigned)(width * (1 - _ratio)), (unsigned)(height * (1 - _ratio)),
		(unsigned)(width * _ratio), unsigned(height * _ratio));
}

void DoubleQbertView::Init()
{
	_views[0]->Init();
	_views[1]->Init();
}

}

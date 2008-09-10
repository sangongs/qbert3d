#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "MathUtils.h"
#include "Point3D.h"

#include "DoubleQbertView.h"

namespace BGComplete
{
	
DoubleQbertView::DoubleQbertView(QbertView_Ptr view1, QbertView_Ptr view2, float ratio) : _ratio(ratio), _state(0)
{
	_views[0] = view1;
	_views[1] = view2;
}

void DoubleQbertView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey)
{
	if (viewKey == 'v')
		_state = !_state;

	_views[0]->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, 0);
	_views[1]->CameraMove(deltaX, deltaY, deltaZ, xRotate, yRotate, zRotate, 0);
}

void DoubleQbertView::SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects)
{
	_views[0]->SetUpDrawModel(modelObjects);
	_views[1]->SetUpDrawModel(modelObjects);
}

void DoubleQbertView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
{
	if (clearAndSwap)
	{
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	_views[_state]->Draw(false, startX, startY, startX + width, startY + height);
	glClear(GL_DEPTH_BUFFER_BIT);
	_views[!_state]->Draw(false, 
		startX + (unsigned)(width * (1 - _ratio)), startY + (unsigned)(height * (1 - _ratio)),
		startX + (unsigned)(width * _ratio), startY + (unsigned)(height * _ratio));

	if (clearAndSwap)
		SDL_GL_SwapBuffers();
}

void DoubleQbertView::Init()
{
	_views[0]->Init();
	_views[1]->Init();
}

}

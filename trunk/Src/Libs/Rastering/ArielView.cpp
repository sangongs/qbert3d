#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "MathUtils.h"
#include "Point3D.h"
#include "QbertModel.h"
#include "QbertGameObject.h"

#include "ArielView.h"

namespace BGComplete
{
	ArielView::ArielView(unsigned z) : _z(z) {}

	void ArielView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char /*viewKey*/)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
		glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
		glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
		glMultMatrixf(_matrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, _matrix);
	}

	void ArielView::Init()
	{
		SimpleQbertView::Init();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, _matrix);
	}

	void ArielView::SetUpCamera()
	{
		glLoadIdentity();
		glTranslatef(0, 0, -z);
		glMultMatrixf(_matrix);
	}
}
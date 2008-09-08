#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Math.h"
#include "Point3D.h"
#include "QbertModel.h"
#include "QbertGameObject.h"

#include "ArielView.h"

namespace BGComplete
{
	ArielView::ArielView()	: _x(0), _y(0), _z(0), _xRotate(0), _yRotate(0), _zRotate(0){}

	void ArielView::CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, bool /*viewChange*/)
	{
		_x += deltaX;
		_y += deltaY;
		_z += deltaZ;
		_xRotate += xRotate;
		_yRotate += yRotate;
		_zRotate += zRotate;
	}

	void ArielView::SetUpCamera(QbertModel::ModelObjects& modelObjects)
	{
		glTranslatef(-_x, -_y, -_z);
		glRotatef(_xRotate, 1.0f, 0.0f, 0.0f);
		glRotatef(_yRotate, 0.0f, 1.0f, 0.0f);
		glRotatef(_zRotate, 0.0f, 0.0f, 1.0f);
	}
}
#pragma once

#include "SimpleQbertView.h"

namespace BGComplete
{
	class ArielView : public SimpleQbertView
	{
	private:
		float _x, _y, _z, _xRotate, _yRotate, _zRotate;

	protected:
		void SetUpCamera();
		void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	
	public:
		ArielView::ArielView(unsigned z);
	};
}

#pragma once

#include "SimpleQbertView.h"

namespace BGComplete
{

class FPSView : public SimpleQbertView
{

protected:
	void SetUpCamera(QbertModel::ModelObjects& modelObjects);
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, bool viewChange);

};

}
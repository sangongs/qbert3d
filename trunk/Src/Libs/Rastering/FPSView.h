#pragma once

#include "ArielView.h"

namespace BGComplete
{

class FPSView : public ArielView
{

protected:
	void SetUpCamera(QbertModel::ModelObjects& modelObjects);
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate);

};

}
#pragma once

#include "QbertView.h"

namespace BGComplete
{

class FPSView : public QbertView
{

protected:
	void SetUpCamera(QbertModel::ModelObjects& modelObjects);
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate);

};

}
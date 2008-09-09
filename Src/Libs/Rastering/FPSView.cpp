#include "StdAfx.h"

#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"

#include "Math.h"
#include "Point3D.h"
#include "QbertModel.h"
#include "QbertGameObject.h"

#include "FPSView.h"

namespace BGComplete
{

void FPSView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char /*viewKey*/)
{
}

void FPSView::SetUpCamera(QbertModel::ModelObjects& modelObjects)
{
	glRotatef(60.0f, 1, 0, 0);
	glTranslatef(0, -3, -1.5);
	glRotatef(180.0f, 0, 1, 0);
	PerformAdditionalTransformations(modelObjects.Qbert, true);
	glTranslatef(-modelObjects.Qbert->Center.X(), -modelObjects.Qbert->Center.Y(), -modelObjects.Qbert->Center.Z());
}

}
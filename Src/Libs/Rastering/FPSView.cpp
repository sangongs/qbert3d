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

void FPSView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/)
{
}

void FPSView::SetUpCamera(QbertModel::ModelObjects& modelObjects)
{

	Math::Point3D rightDirection(modelObjects.Qbert->GetCurrentRightDirection());

	glRotatef(-30.0f, rightDirection.X(), rightDirection.Y(), rightDirection.Z());
	PerformAdditionalTransformations(modelObjects.Qbert, true);
	glTranslatef(
		-modelObjects.Qbert->CurrentUpDirection.X() * 0.7f + modelObjects.Qbert->CurrentFaceDirection.X() * 0.2f -modelObjects.Qbert->Center.X(), 
		-modelObjects.Qbert->CurrentUpDirection.Y() * 0.7f + modelObjects.Qbert->CurrentFaceDirection.Y() * 0.2f -modelObjects.Qbert->Center.Y(),
		-modelObjects.Qbert->CurrentUpDirection.Z() * 0.7f + modelObjects.Qbert->CurrentFaceDirection.Z() * 0.2f -modelObjects.Qbert->Center.Z());
}

}
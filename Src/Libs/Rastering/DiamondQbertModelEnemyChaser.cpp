#include "StdAfx.h"

#include "DiamondQbertModel.h"

#include "DiamondQbertModelEnemyChaser.h"


namespace BGComplete
{

DiamondQbertModelEnemyChaser::DiamondQbertModelEnemyChaser(const std::string& name, Model* model, int score, QbertBox_ptr box, DWORD moveLegth)
	: QbertEnemyObj(name, model, score, box, "chaser", moveLegth) 
{
	SetListOfBoxes();
}

void DiamondQbertModelEnemyChaser::SetListOfBoxes()
{
	int size = static_cast<DiamondQbertModel*>(_model)->Size();
	VecOfAppearanceBox_ptr ret(new std::vector<AppearanceBox>);
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, size - 1, 0),
		Math::Point3D(0.0f, 1.0f, 0.0f), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, - size + 1, 0),
		Math::Point3D(0.0f, -1.0f, 0.0f), Math::Point3D(0.0f, 0.0f, -1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(size - 1, 0, 0),
		Math::Point3D(1.0f, 0.0f, 0.0f), Math::Point3D(0.0f, 1.0f, 0.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(- size + 1, 0, 0),
		Math::Point3D(-1.0f, 0.0f, 0.0f), Math::Point3D(0.0f, -1.0f, 0.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, 0, size - 1),
		Math::Point3D(0.0f, 0.0f, 1.0f), Math::Point3D(1.0f, 0.0f, 0.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(- size + 1, 0, 0),
		Math::Point3D(0.0f, -1.0f, 0.0f), Math::Point3D(-1.0f, 0.0f, 0.0f)));

	_apperanceMap["chaser"] = ret;
}


Direction DiamondQbertModelEnemyChaser::HelperToWhereToMove(bool /*isXAxis*/)
{
// 	Math::Point3D nextFaceDirection(isXAxis ? 0 : static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.X() - Center.X(), 
// 		0, isXAxis ? static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.Z() - Center.Z() : 0);
// 
// 	if (nextFaceDirection == Math::Point3D::Zero)			//standing on the Qbert
// 		return None;
// 	nextFaceDirection.Normalize();
// 
// 	switch (Math::Round(nextFaceDirection.ScalarProduct(LastFaceDirection)))
// 	{
// 	case (1):
// 		return Up;
// 	case (-1):
// 		return Down;
// 	default :
// 		Math::Point3D temp = nextFaceDirection.CrossProduct(LastFaceDirection);
// 		if (nextFaceDirection.CrossProduct(LastFaceDirection).IsEqual(LastUpDirection))
// 			return Right;
// 
// 		return Left;
// 	}
	return None;
}

Direction DiamondQbertModelEnemyChaser::WhereToMove()
{
	return None;
}

}	//namespace BGComplete

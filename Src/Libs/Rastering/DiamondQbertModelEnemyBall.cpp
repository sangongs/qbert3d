#include "StdAfx.h"
#include "DiamondQbertModelEnemyBall.h"

#include "DiamondQbertModel.h"

namespace BGComplete
{

DiamondQbertModelEnemyBall::DiamondQbertModelEnemyBall(const std::string& name, Model* model, QbertBox_ptr box, DWORD moveLegth)
	: QbertEnemyObj(name, model, box, "ball", moveLegth) 
{
	if (_apperanceMap.find("ball") == _apperanceMap.end())
		SetListOfBoxes();
}

void DiamondQbertModelEnemyBall::SetListOfBoxes()
{
	VecOfAppearanceBox_ptr ret(new std::vector<AppearanceBox>);
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, static_cast<DiamondQbertModel*>(_model)->Size() - 1, 0),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, - static_cast<DiamondQbertModel*>(_model)->Size() + 1, 0),
		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));

	_apperanceMap["ball"] = ret;
}

Direction DiamondQbertModelEnemyBall::WhereToMove()
{
	srand( (unsigned)time(NULL) );
	if (LastBox->IsOnPerimeter())
		return IntoBox;
	if (MovingDirection == OutOfBox)
		return Direction(rand() % 4);
	if (MovingDirection == Left)
		return ((rand() % 2) ? Right: Up);
	if (MovingDirection == Right)
		return ((rand() % 2) ? Left: Up);

	return Direction(rand() % 3);
}


}	//namespace BGComplete
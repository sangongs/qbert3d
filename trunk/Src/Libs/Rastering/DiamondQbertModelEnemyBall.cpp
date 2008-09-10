#include "StdAfx.h"

#include "DiamondQbertModel.h"

#include "DiamondQbertModelEnemyBall.h"


namespace BGComplete
{

DiamondQbertModelEnemyBall::DiamondQbertModelEnemyBall(const std::string& name, Model* model, int score, QbertBox_ptr box, DWORD moveLegth)
	: QbertEnemyObj(name, model, score, box, "ball", moveLegth) 
{
	SetListOfBoxes();
}

void DiamondQbertModelEnemyBall::SetListOfBoxes()
{
	int size = static_cast<DiamondQbertModel*>(_model)->Size();
	VecOfAppearanceBox_ptr ret(new std::vector<AppearanceBox>);
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, size - 1, 0),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
 	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, - size + 1, 0),
 		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));

	_apperanceMap["ball"] = ret;
}

Direction DiamondQbertModelEnemyBall::WhereToMove()
{
	boost::mt19937 generator((boost::uint32_t)std::time(0));
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> uniRand12(generator, boost::uniform_int<>(0, 11));

	if (LastBox->IsOnPerimeter())
		return IntoBox;
	if (MovingDirection == OutOfBox)
		return Direction(uniRand12() % 4);
	if (MovingDirection == Left)
		return ((uniRand12() % 2) ? Right: Up);
	if (MovingDirection == Right)
		return ((uniRand12() % 2) ? Left: Up);

	return Direction(uniRand12() % 3);
}


}	//namespace BGComplete
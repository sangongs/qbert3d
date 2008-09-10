#include "StdAfx.h"

#include "DiamondQbertModel.h"

#include "DiamondQbertModelEnemyBall.h"


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
	//[todo] make random work! every where!
	boost::mt19937 generator((boost::uint32_t)std::time(0));
	boost::uniform_int<> uni_dist(0, 11);
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> uniRand(generator, uni_dist);

	if (LastBox->IsOnPerimeter())
		return IntoBox;
	if (MovingDirection == OutOfBox)
		return Direction(uniRand() % 4);
	if (MovingDirection == Left)
		return ((uniRand() % 2) ? Right: Up);
	if (MovingDirection == Right)
		return ((uniRand() % 2) ? Left: Up);

	return Direction(uniRand() % 3);
}


}	//namespace BGComplete
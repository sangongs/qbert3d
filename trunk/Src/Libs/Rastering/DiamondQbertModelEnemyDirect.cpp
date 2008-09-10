#include "StdAfx.h"

#include "DiamondQbertModel.h"

#include "DiamondQbertModelEnemyDirect.h"


namespace BGComplete
{

const float  DiamondQbertModelEnemyDirect::_luckFactor = 0.075f;

DiamondQbertModelEnemyDirect::DiamondQbertModelEnemyDirect(const std::string& name, Model* model, QbertBox_ptr box, DWORD moveLegth)
	: QbertEnemyObj(name, model, box, "directEnemy", moveLegth), _movesToChase(0), _movesOfLuckLeft(0)
{
	if (_apperanceMap.find("directEnemy") == _apperanceMap.end())
		SetListOfBoxes();
}

void DiamondQbertModelEnemyDirect::SetListOfBoxes()
{
	int size = static_cast<DiamondQbertModel*>(_model)->Size();
	VecOfAppearanceBox_ptr ret(new std::vector<AppearanceBox>);
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(1, size - 2, 0),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
 	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(1, - size + 2, 0),
		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(-1, size - 2, 0),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(-1, - size + 2, 0),
		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, size - 2, 1),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, - size + 2, 1),
		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, size - 2, -1),
		Math::Point3D(0, 1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, - size + 2, -1),
		Math::Point3D(0, -1.0f, 0), Math::Point3D(0.0f, 0.0f, 1.0f)));

	_apperanceMap["directEnemy"] = ret;
}

Direction DiamondQbertModelEnemyDirect::HelperToWhereToMove(bool isXAxis)
{
	_movesToChase = 5;
	Math::Point3D nextFaceDirection(isXAxis ? 0 : static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.X() - Center.X(), 
		0, isXAxis ? static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.Z() - Center.Z() : 0);

	if (nextFaceDirection == Math::Point3D::Zero)
		return None;
	nextFaceDirection.Normalize();

	switch (Math::Round(nextFaceDirection.ScalarProduct(LastFaceDirection)))
	{
	case (1):
		return Up;
	case (-1):
		return Down;
	default :
		Math::Point3D temp = nextFaceDirection.CrossProduct(LastFaceDirection);
		if (nextFaceDirection.CrossProduct(LastFaceDirection).IsEqual(LastUpDirection))
			return Right;

		return Left;
	}
}

Direction DiamondQbertModelEnemyDirect::WhereToMove()
{
	boost::mt19937 generator((boost::uint32_t)std::time(0));
	boost::variate_generator<boost::mt19937,  boost::uniform_real<float>> uniRandReal01(generator, boost::uniform_real<float>(0, 1));
	boost::variate_generator<boost::mt19937, boost::uniform_int<>> uniRand4(generator, boost::uniform_int<>(0 ,4));

	if (_movesOfLuckLeft-- > 0)
		return Direction(uniRand4());

	if (uniRandReal01() < _luckFactor)						//Factor of luck, enemy forgets about the Qbert.
	{
		_movesOfLuckLeft = _luckContinuation;
		return Direction(uniRand4());
	}

	if (Center.Y() * static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.Y() > 0)		//Same half (up, down)
	{
 		if (Math::Round(Center.X()) == Math::Round(static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.X()))
			return HelperToWhereToMove(true);																	//The enemy can see the Qbert, same X coordinate
		if (Math::Round(Center.Z()) == Math::Round(static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->LastBox->Center.Z()))
			return HelperToWhereToMove(false);																	//The enemy can see the Qbert, same X coordinate
	}

	if (_movesToChase-- > 0)		//The Enemy Is Chasing the Qbert.
		return Up;

	return Direction(uniRand4());
}

}	//namespace BGComplete
#include "StdAfx.h"

#include "MathUtils.h"
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
	ret->push_back(AppearanceBox(static_cast<QbertModel*>(_model)->GetBoxAt(0, 0, - size + 1),
		Math::Point3D(0.0f, 0.0f, -1.0f), Math::Point3D(-1.0f, 0.0f, 0.0f)));

	_apperanceMap["chaser"] = ret;
}


Direction DiamondQbertModelEnemyChaser::MoveToPoint(const Math::Point3D& moveTo)
{
	Math::Point3D nextFaceDirection;;

	if (moveTo.IsEqual(Center, _point3DEquPrecision))	//If already there.
		return None;

	if (!IsEqulZero(Center.X() - moveTo.X()) && IsEqulZero(Math::Point3D(1, 0, 0).ScalarProduct(LastUpDirection)))
		nextFaceDirection = Math::Point3D(moveTo.X() - Center.X(), 0,  0);
	else if (!IsEqulZero(Center.Z() - moveTo.Z()) && IsEqulZero(Math::Point3D(0, 0, 1).ScalarProduct(LastUpDirection)))
		nextFaceDirection = Math::Point3D(0, 0,  moveTo.Z() - Center.Z());	
	else if (!IsEqulZero(Center.Y() - moveTo.Y()) && IsEqulZero(Math::Point3D(0, 1, 0).ScalarProduct(LastUpDirection)))
		nextFaceDirection = Math::Point3D(0,  moveTo.Y() - Center.Y(), 0);

	if (nextFaceDirection.IsEqual(Point3D::Zero))
		throw std::exception("Can't move like this, moving direction equals to UpDirection in 'DiamondQbertModelEnemyChaser::MoveToPoint()'");

	nextFaceDirection.Normalize();

	switch (Math::Round(nextFaceDirection.ScalarProduct(LastFaceDirection)))
	{
	case (1):
		return Up;
	case (-1):
		return Down;
	default :
		Math::Point3D temp = nextFaceDirection.CrossProduct(LastFaceDirection);
		if (nextFaceDirection.CrossProduct(LastFaceDirection).IsEqual(LastUpDirection, _point3DEquPrecision))
			return Right;

		return Left;
	}
}

Direction DiamondQbertModelEnemyChaser::WhereToMove()
{
	if (IsMoving)
		return None;

	Math::Point3D QbertCenter = static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->NextBox->Center 
		+ static_cast<QbertModel*>(_model)->GetModelObjects()->Qbert->NextUpDirection;
	float sizeOfModel = (float)static_cast<DiamondQbertModel*>(_model)->Size();

	if (((Center.Y() * QbertCenter.Y()) > 0))		//Same half (up, down)
		return MoveToPoint(QbertCenter);
	
	if (IsEqulZero(Center.Y()))						//The Enemy is on the perimeter
	{
		if (IsEqulZero(QbertCenter.Y()))						//Both the Qbert and the Enemy are one the perimeter
		{
			if (IsEqulZero(Center.X()))
				return MoveToPoint(Center + Math::Point3D(Math::Sign(QbertCenter.X()), 0, 0));
			if (IsEqulZero(Center.Z()))
				return MoveToPoint(Center + Math::Point3D(0, 0, Math::Sign(QbertCenter.Z())));


			if ((Center.X() * QbertCenter.X()) >= 0)				//Near quarter on the perimeter
			{
				if(Center.Z() * QbertCenter.Z() >= 0)				//Same quarter on the perimeter
					return MoveToPoint(QbertCenter);
				else
					return MoveToPoint(Center + Math::Point3D(Math::Sign(Center.X()), 0, -Center.Z()));
			}
			
			if ((Center.Z() * QbertCenter.Z()) >= 0)				//Near but not the same quarter on the perimeter
				return MoveToPoint(Center + Math::Point3D(-Center.X(), 0, Math::Sign(Center.Z())));

			return MoveToPoint(Center + Math::Point3D(1, 0, 1));	//Go right to change to another quarter of the perimeter and then think
		}
		else
			return MoveToPoint(Center + Math::Point3D(0, Math::Sign(QbertCenter.Y()), 0));
	}

	else if (IsEqulZero(QbertCenter.Y()))			//Only the Qbert Is on the Perimeter
		return MoveToPoint(QbertCenter);


	else		//different half, tries to find the shortest pass... 
	{
		/*
			To cross the perimeter Enemy has at least to directions, in (+-1, 0, 0) and (0, 0, +-1) directions, 
			try each of those two and decide on the shortest from there to the Qbert.

			To decide on the distance from there to the Qbert lets notice that :
				the X coordinate == Center.X() OR Z == Center.Z() 
					AND
				[abs(Center.X()) + abs(Center.Y()) == sizeOfModel]
		*/

		Math::Point3D direction;
		float minQbertDist;

		if (Center.X() >= 0)
		{
			direction = Math::Point3D(1, 0, 0);
			minQbertDist = std::abs(QbertCenter.Z() - Center.Z()) + std::abs(QbertCenter.X() - (sizeOfModel - std::abs(Center.Z())));
		}
		else
		{
			direction = Math::Point3D(-1, 0, 0);
			minQbertDist = std::abs(QbertCenter.Z() - Center.Z()) + std::abs(QbertCenter.X() + (sizeOfModel - std::abs(Center.Z())));
		}

		if (Center.X() == 0)
		{
			float tempQbertDist =  std::abs(QbertCenter.Z() - Center.Z()) + std::abs(QbertCenter.X() + (sizeOfModel - std::abs(Center.Z())));
			if (tempQbertDist < minQbertDist)
			{
				direction = Math::Point3D(-1, 0, 0);
				minQbertDist = tempQbertDist;
			}
		}

		if (Center.Z() >= 0)
		{
			float tempQbertDist = std::abs(QbertCenter.X() - Center.X()) + std::abs(QbertCenter.Z() - (sizeOfModel - std::abs(Center.X())));
			if (tempQbertDist < minQbertDist)
			{
				direction = Math::Point3D(0, 0, 1);
				minQbertDist = tempQbertDist;
			}
		}
		else
		{
			float tempQbertDist = std::abs(QbertCenter.X() - Center.X()) + std::abs(QbertCenter.Z() + (sizeOfModel - std::abs(Center.X())));
			if (tempQbertDist < minQbertDist)
			{
				direction = Math::Point3D(0, 0, -1);
				minQbertDist = tempQbertDist;
			}
		}

		if (Center.Z() == 0)
		{
			float tempQbertDist =  std::abs(QbertCenter.X() - Center.X()) + std::abs(QbertCenter.Z() + (sizeOfModel - std::abs(Center.X())));
			if (tempQbertDist < minQbertDist)
			{
				direction = Math::Point3D(0, 0, -1);
				minQbertDist = tempQbertDist;
			}
		}

		return MoveToPoint(Center + direction);
	}
}

}	//namespace BGComplete

#include "StdAfx.h"

#include <string>

#include "MathUtils.h"
#include "Point3D.h"

#include "QbertModel.h"
#include "SimpleControler.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"
#include "DiamondQbertEnemiesFactory.h"

#include "DiamondQbertModel.h"

namespace BGComplete
{

DiamondQbertModel::DiamondQbertModel(int sizeOfDiamond, const std::string& unvisitedBoxName, const std::string& visitedBoxName, const std::string& qbertName,
	int * score, int * livesLeft, float freeFallAcceleration)
	: QbertModel(unvisitedBoxName, visitedBoxName, score, livesLeft, freeFallAcceleration), _size(sizeOfDiamond)
{
	CreateDiamondStructure(sizeOfDiamond);
	SetupQbert(sizeOfDiamond, qbertName);
}

DiamondQbertModel::~DiamondQbertModel(void){}

void DiamondQbertModel::CreateDiamondStructure(int sizeOfDiamond)
{
	for (int i = 0, jBorder = sizeOfDiamond - 1; i < sizeOfDiamond; i++, jBorder--)
	{
		for (int j = -jBorder; j <= jBorder; j++)
		{
			for (int face = 0; face < 4; face++)
			{
				bool isPerimetar = false;

				if (!i)
				{
					isPerimetar = true;
					if(face == 2)														// Draw the 0 level only once; Beware, dosen't draw top and lower levels.
						break;
				}

				if ((j == -jBorder || j == jBorder) && face % 2 == 0)					// Boxes between faces has to be drawn once.
					continue;

				float x = (float)j, 
					y = ((face & 2) ? -1 : 1) * (float)i,
					z = ((face & 1) ? -1 : 1) * (float)(sizeOfDiamond - 1 - i - std::abs(j));

				Math::Point3D upDirection(0, (face & 2) ? -1.0f : 1.0f, 0);

				InsertBox(Math::Point3D(x, y, z), QbertBox_ptr(new QbertBox(_unvisitedBoxName, face, isPerimetar, x, y, z)));
				_boxesUnvisited++;
			}
		}
	}
}

void DiamondQbertModel::SetupQbert(int sizeOfDiamond, const std::string& qbertName)
{
	SetQbert(qbertName, _startingBox = _objects->BoxMap.find(Math::Point3D(0.0f , (float)sizeOfDiamond - 1.0f, 0.0f))->second);
	_objects->Qbert->SetModel(this);


	_objects->Qbert->LastUpDirection = Math::Point3D(0, 1, 0);
	_objects->Qbert->LastFaceDirection = Math::Point3D(0, 0, 1);
	_objects->Qbert->NextUpDirection = Math::Point3D(0, 1, 0);
	_objects->Qbert->NextFaceDirection = Math::Point3D(0, 0, 1);
	_objects->Qbert->SetMoveLength(1000, _freeFallAcceleration);
}

void DiamondQbertModel::ReciveInput(const SimpleControler::InputData& inputData)
{
	Move(_objects->Qbert, inputData);
	CreateEnemies(inputData.DeltaTime);
	MoveEnemies(inputData.DeltaTime);
}

void DiamondQbertModel::Move(QbertGameObject_ptr object, const SimpleControler::InputData& inputData)
{
	object->Center = object->LastBox->Center + object->LastUpDirection;

	object->CurrentFaceDirection = object->LastFaceDirection;
	object->CurrentUpDirection = object->LastUpDirection;


	if (object->IsMoving)
	{
		object->Progress += (float)inputData.DeltaTime / (float)object->GetMoveLength();

		UpdateCenterOfObject(object);
		UpdateFaceAndUpDirections (object);
	}
	else if (inputData.direction != None)		//!object->IsMoving && inputData.direction != None
	{
		object->MovingDirection = inputData.direction;
		ChangeBox(object);

		object->IsMoving = true;
	}

	if (object->Progress > 1)
		EndMovement(object);
}

void DiamondQbertModel::UpdateCenterOfObject(QbertGameObject_ptr object)
{
	if (object->IsChangingBox)
	{
		object->Center += object->GetHorizontalSpeed() * object->Progress * object->NextFaceDirection;	

		float progress = (object->IsMovingUp) ? 1 - object->Progress : object->Progress;

		object->Center += object->LastUpDirection * (
			(object->IsMovingUp ? 1 : 0)
			+ object->GetVerticalSpeed() * progress 
			- progress * progress * object->GetFreeAcceleration() * 0.5f);
	}
	else if (object->MovingDirection == OutOfBox)
		object->Center -= (1 - object->Progress) * object->NextUpDirection;
	else if (object->MovingDirection == IntoBox)
		object->Center -= object->Progress * object->NextUpDirection;
	else
		object->Center += object->NextUpDirection * (1 + Math::DCos(180 + 270 * object->Progress)) 
			+ object->LastUpDirection * Math::DSin(270 * object->Progress);
}

void DiamondQbertModel::UpdateFaceAndUpDirections(QbertGameObject_ptr object)
{
	float factor1 = Math::DCos(90 * object->Progress), factor2 = Math::DSin(90 * object->Progress);
	if (object->IsChangingBox)
	{
		switch(object->MovingDirection)
		{
		case Left:
		case Right:
			object->CurrentFaceDirection = object->LastFaceDirection * factor1 + 
				object->NextFaceDirection * factor2;
			break;
		case Down:
			factor1 = Math::DCos(180 * object->Progress);
			factor2 = Math::DSin(180 * object->Progress);
			object->CurrentFaceDirection = object->LastFaceDirection * factor1 
				+ object->GetRightDirection() * factor2;
		}
	}
	else
	{
		object->CurrentUpDirection = object->LastUpDirection * factor1 + 
			object->NextUpDirection * factor2;

		switch(object->MovingDirection)
		{
		case Up:
			object->CurrentFaceDirection = object->LastFaceDirection * factor1 + 
				object->NextFaceDirection * factor2;
			break;
		case Down:
			object->CurrentFaceDirection = object->LastFaceDirection * factor1 - 
				object->NextFaceDirection * factor2;

			factor1 = Math::DCos(180 * object->Progress);
			factor2 = Math::DSin(180 * object->Progress);
			object->CurrentFaceDirection = object->CurrentFaceDirection * factor1 + 
				object->CurrentFaceDirection.CrossProduct(object->CurrentUpDirection) * factor2;
			break;
		case Right:
		case Left:
			object->CurrentFaceDirection = object->CurrentFaceDirection * factor1 + 
				((object->MovingDirection == Right) ? factor2 : -factor2) * object->CurrentFaceDirection.CrossProduct(object->CurrentUpDirection);
		}
	}
}

/**	
 *  @pre (object->MovingDirection != NULL && 
 *		object->MovingDirection != OutOfBox &&
 *		object->MovingDirection != InsideABox)
**/
void DiamondQbertModel::ChangeBox(QbertGameObject_ptr object)
{
	Math::Point3D newBoxCoordinate(object->LastBox->Center), rightDirection = object->GetRightDirection();

	object->IsChangingBox = true;

	switch (object->MovingDirection)
	{
	case Up:
		newBoxCoordinate += object->LastFaceDirection;
		break;
	case Down:
		newBoxCoordinate -=  object->LastFaceDirection;
		object->NextFaceDirection = object->LastFaceDirection * (-1);
		break;
	case Right:	
	case Left:
		newBoxCoordinate += (object->MovingDirection == Right ? 1.0f : -1.0f) * rightDirection;
		object->NextFaceDirection = (object->MovingDirection == Right ? 1.0f : -1.0f) * rightDirection;
		break;
	case IntoBox:
	case OutOfBox:
		object->IsChangingBox = false;
		object->NextBox = object->LastBox;
		return;
	}

	std::map<Math::Point3D, QbertBox_ptr>::iterator box = _objects->BoxMap.find(newBoxCoordinate + object->LastUpDirection);
	if (box != _objects->BoxMap.end())
	{
		object->IsMovingUp = true;
		object->IsChangingBox = true;
		object->NextBox = box->second;

		if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//standing on the perimeter, facing out of the diamond, 
			object->IsChangingBox = false;													//can't move in one move out of the perimeter.
		else
			return;
	}

	if (object->IsChangingBox &&
		(box = _objects->BoxMap.find(newBoxCoordinate - object->LastUpDirection)) != _objects->BoxMap.end())
	{
		object->IsMovingUp = false;
		object->IsChangingBox = true;
		object->NextBox = box->second;

		if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//standing on the perimeter, facing out of the diamond, 
			object->IsChangingBox = false;													//can't move in one move out of the perimeter.
		else 
			return;
	}

	//Not Changing a box!
	object->IsChangingBox = false;
	object->NextBox = object->LastBox;

	object->NextFaceDirection = -object->LastUpDirection;
	switch (object->MovingDirection)
	{
	case Up:
	case Down:
		object->NextUpDirection = (object->MovingDirection == Up ? 1.0f : -1.0f) * object->LastFaceDirection;
		break;
	default:				//can't be None, OutOfBox, InsideABox: @pre of the function
		object->NextUpDirection = (object->MovingDirection == Right ? 1.0f : -1.0f) *  rightDirection;
	}
}

void DiamondQbertModel::EndMovement(QbertGameObject_ptr object)
{
	if (object->MovingDirection == IntoBox)
	{
		if (object->IsQbert())
			throw std::exception("Qbert can't be removed from the model! (in function 'DiamondQbertModel::EndMovement()'");

		_enemiesToDelete.push_back(boost::static_pointer_cast<QbertEnemyObj>(object));
		_score += 75;
		return;
	}

	object->LastBox = object->NextBox;
	object->LastUpDirection = object->NextUpDirection;
	object->LastFaceDirection = object->NextFaceDirection;

	if (object->IsQbert())
		VisitBox(object->LastBox);

	object->Center = object->LastBox->Center + object->LastUpDirection;

	object->CurrentFaceDirection = object->LastFaceDirection;
	object->CurrentUpDirection = object->LastUpDirection;

	object->Progress = 0;
	object->IsMoving = false;
}

void DiamondQbertModel::MoveEnemies(DWORD deltaTime)
{
	BOOST_FOREACH(QbertEnemyObj_ptr iter, _objects->Enemies)
	{
		Move(iter, SimpleControler::InputData(deltaTime, iter->WhereToMove()));
		if (iter->IsQbertStillDies())
		{
			_enemiesToDelete = _objects->Enemies;
			_livesLeft--;
			break;
		}
	}

	BOOST_FOREACH(QbertEnemyObj_ptr iter, _enemiesToDelete)
	{
		RemoveEnemy(iter);
	}
	_enemiesToDelete.erase(_enemiesToDelete.begin(), _enemiesToDelete.end());
}

void DiamondQbertModel::RemoveEnemy(const QbertEnemyObj_ptr iter)
{
	_objects->Enemies.remove(iter);
	BOOST_FOREACH(EnemiesAppearanceData& data, _enemiesAppearanceData)
		if (data.Type == iter->GetType())
		{
			data.TotalAmount--;
			return;
		}
}

void DiamondQbertModel::AddNewEnemyType(const std::string& type, const std::string& name, DWORD firstDelay, DWORD appearanceFrequency, DWORD moveLength, int maxAppearances)
{
	_enemiesAppearanceData.push_back(EnemiesAppearanceData (type, name, firstDelay, appearanceFrequency, moveLength, maxAppearances));
}

void DiamondQbertModel::CreateEnemies (DWORD deltaTime)
{
	BOOST_FOREACH (EnemiesAppearanceData& data, _enemiesAppearanceData)
	{
		data.TimeSinceLastAppearance += deltaTime;
		if (data.TimeSinceLastAppearance > ((data.IsAppearedOnce) ? data.FirstDelay : data.AppearanceFrequency))
		{
			if (data.TotalAmount >= data.MaxAppearances)
				continue;

			QbertEnemyObj_ptr newEnemy = DiamondQbertEnemiesFactory::GetNewEnemy(data.Type, data.Name, this);

			SetEnemysMoveLength(newEnemy, data.MoveLength);

			VecOfAppearanceBox_ptr vectorOfBoxes = newEnemy->GetAppearanceBoxes();

			boost::mt19937 generator((boost::uint32_t)std::time(0));
			boost::variate_generator<boost::mt19937, boost::uniform_int<>> uniRand(generator, boost::uniform_int<>(0, (int)vectorOfBoxes->size() - 1));

			int index = uniRand();

			newEnemy->IsMoving = true;
			newEnemy->IsChangingBox = false;
			newEnemy->MovingDirection = OutOfBox;
			newEnemy->LastBox = (*vectorOfBoxes)[index].get<0>();
			newEnemy->LastUpDirection = (*vectorOfBoxes)[index].get<1>();
			newEnemy->LastFaceDirection = (*vectorOfBoxes)[index].get<2>();
			newEnemy->NextBox = newEnemy->LastBox;
			newEnemy->NextUpDirection = newEnemy->LastUpDirection;
			newEnemy->NextFaceDirection = newEnemy->LastFaceDirection;


			data.TimeSinceLastAppearance = 0;

			_objects->Enemies.push_back(newEnemy);

			data.TotalAmount++;
			data.IsAppearedOnce = true;
		}
	}
}

}	//End of namespace BGComplete

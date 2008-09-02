#include "StdAfx.h"

#include <string>

#include "Point3D.h"

#include "QbertModel.h"
#include "SimpleControler.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

#include "DiamondQbertModel.h"


DiamondQbertModel::DiamondQbertModel(int sizeOfDiamond, const std::string unvisitedBoxName, const std::string visitedBoxName, const std::string qbertName, float freeFallAcceleration)
: QbertModel(unvisitedBoxName, visitedBoxName, freeFallAcceleration)
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

				float x, y, z;
				Point3D upDirection(0, 1, 0);
				switch(face)
				{
				case (0):
					x = (float)j;
					y = (float)i;
					z = (float)(sizeOfDiamond - 1 - i - std::abs(j));
					break;
				case (1):
					x = (float)j;
					y = (float)i;
					z = -(float)(sizeOfDiamond - 1 - i - std::abs(j));
					break;
				case (2):
					x = (float)j;
					y = (float)-i;
					z = (float)(sizeOfDiamond - 1 - i - std::abs(j));
					upDirection.Points[1] = -1;
					break;
				default:				//case(3):
					x = (float)j;
					y = (float)-i;
					z = -(float)(sizeOfDiamond - 1 - i - std::abs(j));
					upDirection.Points[1] = -1;
				}

				InsertBox(Point3D(x, y, z), QbertBox_ptr(new QbertBox(_unvisitedBoxName, face, isPerimetar, x, y, z)));
				_boxesUnvisited++;
			}
		}
	}
}

void DiamondQbertModel::SetupQbert(int sizeOfDiamond, const std::string& qbertName)
{
	SetQbert(qbertName, _startingBox = _objects.BoxMap.find(Point3D(0.0f , (float)sizeOfDiamond - 1.0f, 0.0f))->second);
	_objects.Qbert->SetModel(this);


	_objects.Qbert->LastUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->LastFaceDirection = Point3D(0, 0, 1);
	_objects.Qbert->NextUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->NextFaceDirection = Point3D(0, 0, 1);
	_objects.Qbert->SetMoveLength(1000, _freeFallAcceleration);
}

void DiamondQbertModel::ReciveInput(const SimpleControler::InputData& inputData)
{
	Move(_objects.Qbert, inputData);
	MakeEnemiesMove(inputData.DeltaTime);
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
	else
	{
		if (inputData.direction != None)
		{
			object->MovingDirection = inputData.direction;
			ChangeBox(object);

			object->IsMoving = true;
		}
	}

	if (object->Progress > 1)
		EndMovment(object);
}

void DiamondQbertModel::UpdateCenterOfObject(QbertGameObject_ptr object)
{
	if (object->IsChangingBox)
	{
		object->Center += object->GetHorizontalSpeed() * object->Progress * object->NextFaceDirection;	

		float progress = (object->IsMovingUp) ? 1 - object->Progress : object->Progress;

		object->Center += object->LastUpDirection * 
			((object->IsMovingUp ? 1 : 0) +
			object->GetVerticalSpeed() * progress - 
			progress * progress * object->GetFreeAcceleration() * 0.5f);
	}
	else
		object->Center += object->NextUpDirection * (1 + DCos(180 + (270 * object->Progress))) 
		+ object->LastUpDirection * (DSin(270 * object->Progress));
}

void DiamondQbertModel::UpdateFaceAndUpDirections(QbertGameObject_ptr object)
{
	float factor1 = DCos(90 * object->Progress), factor2 = DSin(90 * object->Progress); //[todo] maybe, change name.
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
			factor1 = DCos(180 * object->Progress);
			factor2 = DSin(180 * object->Progress);
			object->CurrentFaceDirection = object->LastFaceDirection * factor1 + 
				object->GetRightDirection() * factor2;
			break;
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

			factor1 = DCos(180 * object->Progress);
			factor2 = DSin(180 * object->Progress);
			object->CurrentFaceDirection = object->CurrentFaceDirection * factor1 + 
				object->CurrentFaceDirection.CrossProduct(object->CurrentUpDirection) * factor2;
			break;
		case Right:
		case Left:
			object->CurrentFaceDirection = object->CurrentFaceDirection * factor1 + ((object->MovingDirection == Right) ? 1.0f : -1.0f) *
				object->CurrentFaceDirection.CrossProduct(object->CurrentUpDirection) * factor2;
			break;
		}
	}
}

void DiamondQbertModel::ChangeBox(QbertGameObject_ptr object)
{
	bool isNotFinished = true;
	Point3D newBoxCoordinate(object->LastBox->Center), rightDirection = object->GetRightDirection();

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
		newBoxCoordinate += rightDirection;
		object->NextFaceDirection = rightDirection;
		break;
	case Left:
		newBoxCoordinate -= rightDirection;
		object->NextFaceDirection = -rightDirection;
	}

	if (isNotFinished)
	{
		std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxMap.find(newBoxCoordinate + object->LastUpDirection);
		if (box != _objects. BoxMap.end())
		{
			object->IsMovingUp = true;
			object->IsChangingBox = true;
			object->NextBox = box->second;

			if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//cant move like this, must stay on the same box
				object->IsChangingBox = false;

			isNotFinished = false;
		}
	}

	if (isNotFinished)
	{
		std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxMap.find(newBoxCoordinate - object->LastUpDirection);
		if (box != _objects. BoxMap.end())
		{
			object->IsMovingUp = false;
			object->IsChangingBox = true;
			object->NextBox = box->second;

			if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//cant move like this, must stay on the same box
				object->IsChangingBox = false;

			isNotFinished = false;
		}
	}

	if (isNotFinished || !object->IsChangingBox)									//Staying on the same box on the perimeter;
	{
		if(!object->LastBox->IsOnPerimeter())
			throw std::exception("Not on perimeter and can't find a place to move on (in function DiamondQbertModel::QbertMove()");

		object->IsChangingBox = false;
		object->NextBox = object->LastBox;

		object->NextFaceDirection = -object->LastUpDirection;
		switch (object->MovingDirection)
		{
		case Up:
		case Down:
			object->NextUpDirection = (object->MovingDirection == Up ? 1.0f : -1.0f) * object->LastFaceDirection;
			break;
		default:										//can't be None.
			object->NextUpDirection = (object->MovingDirection == Right ? 1.0f : -1.0f) *  rightDirection;
		}
	}
}

void DiamondQbertModel::EndMovment(QbertGameObject_ptr object)
{
	object->LastBox = object->NextBox;
	object->LastUpDirection = object->NextUpDirection;
	object->LastFaceDirection = object->NextFaceDirection;

	VisitBox(object->LastBox);

	object->Center = object->LastBox->Center + object->LastUpDirection;

	object->CurrentFaceDirection = object->LastFaceDirection;
	object->CurrentUpDirection = object->LastUpDirection;

	object->Progress = 0;
	object->IsMoving = false;
}

void DiamondQbertModel::MakeEnemiesMove(DWORD deltaTime)
{
	for (std::list<QbertEnemyObj_ptr>::iterator iter = _objects.Enemies.begin(); iter != _objects.Enemies.end(); iter++)
	{
		Move((*iter), SimpleControler::InputData(deltaTime, (*iter)->WhereToMove()));
		if (_isQbertAlive)
			_isQbertAlive = (*iter)->IsQbertStillAlive();
	}
}

#include "StdAfx.h"

#include <string>

#include "QbertModel.h"
#include "SimpleControler.h"

#include "QbertBox.h"
#include "QbertGameObject.h"
#include "QbertEnemyObj.h"

#include "DiamondQbertModel.h"


DiamondQbertModel::DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName, float freeFallAcceleration)
	: QbertModel(boxNameBefore, boxNameAfter, freeFallAcceleration)
{
	this;
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

				InsertBox(Point3D(x, y, z), QbertBox_ptr(new QbertBox(boxNameBefore, face, isPerimetar, x, y, z)));
				_boxesUnvisited++;
			}
		}
	}

	SetQbert(qbertName, _startingBox = _objects.BoxesMap.find(Point3D(0, (float)sizeOfDiamond - 1, 0))->second);
	_objects.Qbert->SetModel(this);


	_objects.Qbert->LastUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->LastFaceDirection = Point3D(0, 0, 1);
	_objects.Qbert->NextUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->NextFaceDirection = Point3D(0, 0, 1);
	_objects.Qbert->SetMoveLength(1000, _freeFallAcceleration);
}

void DiamondQbertModel::ReciveInput(const SimpleControler::InputData& inputData)
{
	/*
		Redesign:
			0] Each object has a shared_ptr to a model.
			1] Enemy objects has a WhereToMove() function which returns input data.
	*/

	Move(_objects.Qbert, inputData);
	MakeEnemiesMove(inputData.DeltaTime);
}

void DiamondQbertModel::Move(QbertGameObject_ptr object, const SimpleControler::InputData& inputData)
{

	Point3D center(object->LastBox->X, object->LastBox->Y, object->LastBox->Z);
	center += object->LastUpDirection;

	object->CurrentFaceDirection = object->LastFaceDirection;
	object->CurrentUpDirection = object->LastUpDirection;


	if (object->IsMoving)
	{
		object->Progress += (float)inputData.DeltaTime / (float)object->GetMoveLength();

		if (object->IsChangingBox)
		{
			center += object->GetHorizontalSpeed() * object->Progress * object->NextFaceDirection;	

			float progress = (object->IsMovingUp) ? 1 - object->Progress : object->Progress;


			center += object->LastUpDirection * 
				((object->IsMovingUp ? 1 : 0) +
				object->GetVerticalSpeed() * progress - 
				progress * progress * object->GetFreeAcceleration() * 0.5f);
		}
		else
		{
			center += object->NextUpDirection * (1 + DCos(180 + (270 * object->Progress)));
			center += object->LastUpDirection * (DSin(270 * object->Progress));
		}

		float factor1 = DCos(90 * object->Progress);
		float factor2 = DSin(90 * object->Progress);
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
	else
	{
		object->CurrentFaceDirection = object->LastFaceDirection;
		object->CurrentUpDirection = object->LastUpDirection;

		if (inputData.direction != None)
		{
			object->IsMoving = true;
			object->MovingDirection = inputData.direction;


			bool isNotFinished = true, isStayingOnTheSameBox = false;			

			Point3D tempCoordiante = object->LastBox->GetCoordinates(), rightDirection = object->GetRightDirection();

			switch (inputData.direction)
			{
			case Up:
				tempCoordiante += object->LastFaceDirection;
				break;
			case Down:
				tempCoordiante -=  object->LastFaceDirection;
				object->NextFaceDirection = object->LastFaceDirection * (-1);
				break;
			case Right:
				tempCoordiante += rightDirection;
				object->NextFaceDirection = rightDirection;
				break;
			case Left:
				tempCoordiante -= rightDirection;
				object->NextFaceDirection = -rightDirection;
				break;
			}

			if (isNotFinished)
			{
				std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxesMap.find(tempCoordiante + object->LastUpDirection);
				if (box != _objects. BoxesMap.end())
				{
					object->IsMovingUp = true;
					object->IsChangingBox = true;
					object->NextBox = box->second;

					if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//cant move like this, must stay on the same box
						isStayingOnTheSameBox = true;

					isNotFinished = false;
				}
			}

			if (isNotFinished)
			{
				std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxesMap.find(tempCoordiante - object->LastUpDirection);
				if (box != _objects. BoxesMap.end())
				{
					object->IsMovingUp = false;
					object->IsChangingBox = true;
					object->NextBox = box->second;

					if((!object->LastUpDirection.Y()) && (!object->NextBox->IsOnPerimeter()))			//cant move like this, must stay on the same box
						isStayingOnTheSameBox = true;

					isNotFinished = false;
				}
			}

			if (isNotFinished || isStayingOnTheSameBox)									//Staying on the same box on the perimeter;
			{
				if(!object->LastBox->IsOnPerimeter())
					throw std::exception("Not on perimeter and can't find a place to move on (in function DiamondQbertModel::QbertMove()");

				object->IsChangingBox = false;
				object->NextBox = object->LastBox;
				switch (inputData.direction)
				{
				case Up:
					object->NextFaceDirection = -object->LastUpDirection;
					object->NextUpDirection = object->LastFaceDirection;
					break;
				case Down:
					object->NextFaceDirection = -object->LastUpDirection;
					object->NextUpDirection = -object->LastFaceDirection;
					break;
				case Right:
					object->NextFaceDirection = -object->LastUpDirection;
					object->NextUpDirection = rightDirection;
					break;
				case Left:
					object->NextFaceDirection = -object->LastUpDirection;
					object->NextUpDirection = -rightDirection;
					break;
				}

				isNotFinished = false;
			}

			object->IsMoving = true;
		}
	}


	object->X = center.X();
	object->Y = center.Y();
	object->Z = center.Z();


	if (object->Progress > 1)
	{	
		object->LastBox = object->NextBox;
		object->LastUpDirection = object->NextUpDirection;
		object->LastFaceDirection = object->NextFaceDirection;

		VisitBox(object->LastBox);

		object->Progress = 0;
		object->IsMoving = false;
	}

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

std::list<GameObject_ptr>* DiamondQbertModel::GetObjects()
{
	_objects.ObjectsList = std::list<GameObject_ptr>();

	return &_objects.ObjectsList;

}

DiamondQbertModel::~DiamondQbertModel(void)
{
}

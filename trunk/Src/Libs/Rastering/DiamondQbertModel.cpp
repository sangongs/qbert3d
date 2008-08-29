#include "StdAfx.h"

#include <string>

#include "QbertModel.h"
#include "SimpleControler.h"

#include "DiamondQbertModel.h"


DiamondQbertModel::DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName)
	: QbertModel(boxNameBefore, boxNameAfter)
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

				InsertBox(Point3D(x, y, z), QbertBox_ptr(new QbertBox(boxNameBefore, face, isPerimetar, x, y, z)));
				_boxesUnvisited++;
			}
		}
	}

	_objects.BoxesMap.find(Point3D(0, (float)sizeOfDiamond - 1, 0))->second->Name = boxNameAfter;
	SetQbert(qbertName, _startingBox = _objects.BoxesMap.find(Point3D(0, (float)sizeOfDiamond - 1, 0))->second.get());


	_objects.Qbert->NowUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->NowFaceDirection = Point3D(0, 0, 1);
	_objects.Qbert->AfterUpDirection = Point3D(0, 1, 0);
	_objects.Qbert->AfterFaceDirection = Point3D(0, 0, 1);
}

void DiamondQbertModel::ReadInput(const SimpleControler::InputData& inputData)			//Deals with time and decides whether to move or not.
{
	QbertMove(inputData);
	MakeEnemiesMove(inputData.DeltaTime);
}

void DiamondQbertModel::QbertMove(const SimpleControler::InputData& inputData)
{
	_objects.Qbert->Progress += (float)(inputData.DeltaTime / _objects.Qbert->MoveLength);

	if (_objects.Qbert->Progress > 1)
 	{

// 		if (_objects.Qbert->NowOn != _objects.Qbert->AfterOn)				//if the Qbert had moved.
// 		{
// 			_objects.Qbert->NowOn = _objects.Qbert->AfterOn;
// 			_objects.Qbert->NowOn->Name = _boxNameAfter;
// 			VisitBox(_objects.Qbert->NowOn);
// 		}
// 		_objects.Qbert->AfterOn = GetBoxNeibhor(_objects.Qbert->NowOn, inputData.direction);
// 		

		bool isNotFinished = true, isMovingFromPerimeterToPerimeter = false, isStayingOnTheSameBox = false;

		_objects.Qbert->NowOn = _objects.Qbert->AfterOn;
		_objects.Qbert->NowUpDirection = _objects.Qbert->AfterUpDirection;
		_objects.Qbert->NowFaceDirection = _objects.Qbert->AfterFaceDirection;
		

		Point3D tempCoordiante = _objects.Qbert->NowOn->GetCoordinates(), rightDirection = _objects.Qbert->GetRightDirection();

		switch (inputData.direction)
		{
		case None:
			isNotFinished = false;
			_objects.Qbert->AfterUpDirection = _objects.Qbert->NowUpDirection;
			_objects.Qbert->AfterFaceDirection = _objects.Qbert->NowFaceDirection;
			_objects.Qbert->AfterOn = _objects.Qbert->NowOn;
			break;
		case Up:
			tempCoordiante += _objects.Qbert->NowFaceDirection;
			break;
		case Down:
			tempCoordiante -=  _objects.Qbert->NowFaceDirection;
			_objects.Qbert->AfterFaceDirection = _objects.Qbert->NowFaceDirection * (-1);
			break;
		case Right:
			tempCoordiante += rightDirection;
			_objects.Qbert->AfterFaceDirection = rightDirection;
			break;
		case Left:
			tempCoordiante += rightDirection;
			_objects.Qbert->AfterFaceDirection = -rightDirection;
			break;
		}

		if (isNotFinished)
		{
			std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxesMap.find(tempCoordiante + _objects.Qbert->NowUpDirection);
			if (box != _objects. BoxesMap.end())
			{
				_objects.Qbert->AfterOn = box->second.get();

				if((!_objects.Qbert->NowUpDirection.Y()) && (!_objects.Qbert->AfterOn->IsOnPerimeter()))			//cant move like this, must stay on the same box
					isStayingOnTheSameBox = true;

				isNotFinished = false;
			}
		}

		if (isNotFinished)
		{
			std::map<Point3D, QbertBox_ptr>::iterator box = _objects. BoxesMap.find(tempCoordiante - _objects.Qbert->NowUpDirection);
			if (box != _objects. BoxesMap.end())
			{
				_objects.Qbert->AfterOn = box->second.get();

				if((!_objects.Qbert->NowUpDirection.Y()) && (!_objects.Qbert->AfterOn->IsOnPerimeter()))			//cant move like this, must stay on the same box
					isStayingOnTheSameBox = true;

				isNotFinished = false;
			}
		}

		if (isNotFinished || isStayingOnTheSameBox)									//Staying on the same box on the perimeter;
		{
			if(!_objects.Qbert->NowOn->IsOnPerimeter())
				throw std::exception("Not on perimeter and can't find a place to move on (in function DiamondQbertModel::QbertMove()");

			_objects.Qbert->AfterOn = _objects.Qbert->NowOn;
			switch (inputData.direction)
			{
			case Up:
				_objects.Qbert->AfterFaceDirection = -_objects.Qbert->NowUpDirection;
				_objects.Qbert->AfterUpDirection = _objects.Qbert->NowFaceDirection;
				break;
			case Down:
				_objects.Qbert->AfterFaceDirection = _objects.Qbert->NowUpDirection;
				_objects.Qbert->AfterUpDirection = -_objects.Qbert->NowFaceDirection;
				break;
			case Right:
				_objects.Qbert->AfterFaceDirection = -_objects.Qbert->NowUpDirection;
				_objects.Qbert->AfterUpDirection = rightDirection;
				break;
			case Left:
				_objects.Qbert->AfterFaceDirection = -_objects.Qbert->NowUpDirection;
				_objects.Qbert->AfterUpDirection = rightDirection;
				break;
			}

			isNotFinished = false;
		}


		VisitBox(_objects.Qbert->NowOn);

		while (_objects.Qbert->Progress > 1)
			_objects.Qbert->Progress--;
	}

}
void DiamondQbertModel::MakeEnemiesMove(DWORD deltaTime)
{
	for (std::list<QbertEnemyObj_ptr>::iterator iter = _objects.Enemies.begin(); iter != _objects.Enemies.end(); iter++)
		(*iter)->Move(*this, deltaTime);
}

std::list<GameObject_ptr>* DiamondQbertModel::GetObjects()
{
	_objects.ObjectsList = std::list<GameObject_ptr>();

	return &_objects.ObjectsList;

}

DiamondQbertModel::~DiamondQbertModel(void)
{
}

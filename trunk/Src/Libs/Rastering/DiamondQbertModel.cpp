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
				if (!i && face == 2)													// Draw the 0 level only once; Beware, dosen't draw top and lower levels.
					break;

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

				InsertBox(Point3D(x, y, z), QbertBox_ptr(new QbertBox(boxNameBefore, upDirection, face, x, y, z)));
				_boxesUnvisited++;
			}
		}
	}

	_objects.BoxesMap.find(Point3D(0, (float)sizeOfDiamond - 1, 0))->second->Name = boxNameAfter;
	SetQbert(qbertName, _startingBox = _objects.BoxesMap.find(Point3D(0, (float)sizeOfDiamond - 1, 0))->second.get());
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
		if (_objects.Qbert->NowOn != _objects.Qbert->AfterOn)				//if the Qbert had moved.
		{
			_objects.Qbert->NowOn = _objects.Qbert->AfterOn;
			_objects.Qbert->NowOn->Name = _boxNameAfter;
			VisitBox(_objects.Qbert->NowOn);
		}
		_objects.Qbert->AfterOn = GetBoxNeibhor(_objects.Qbert->NowOn, inputData.direction);
		

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

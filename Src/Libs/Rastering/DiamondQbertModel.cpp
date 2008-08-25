#include "StdAfx.h"

#include <string>

#include "QbertModel.h"
#include "QbertControler.h"

#include "DiamondQbertModel.h"


DiamondQbertModel::DiamondQbertModel(int sizeOfDiamond, const std::string boxNameBefore, const std::string boxNameAfter, const std::string qbertName)
	: QbertModel(boxNameBefore, boxNameAfter)
{
	for (int i = 0; i < sizeOfDiamond; i++)
	{
		for (int j = - (sizeOfDiamond - i) + 1; j <= (sizeOfDiamond - i) - 1 - 1; j++)	//the last on in the row will be the first one in the row of the next face;
		{
			for (int face = 0; face < 8; face++)
			{
				if (!i && face == 4)													// Draw the 0 level only once;
					break;
				float x, y, z;
				Point3D upDirection(1, 0, 0);
				switch(face)
				{
				case (0):
					x = (float)j;
					y = (float)i;
					z = (float)-i;
					break;
				case (1):
					x = (float)-i;
					y = (float)i;
					z = (float)j;
					break;
				case (2):
					x = (float)j;
					y = (float)i;
					z = (float)i;
					break;
				case (3):
					x = (float)i;
					y = (float)i;
					z = (float)j;
					break;
				case (4):
					x = (float)j;
					y = (float)i;
					z = (float)-i;
					upDirection.Points[0] = -1;
					break;
				case (5):
					x = (float)-i;
					y = (float)i;
					z = (float)j;
					upDirection.Points[0] = -1;
					break;
				case (6):
					x = (float)j;
					y = (float)i;
					z = (float)i;
					upDirection.Points[0] = -1;
					break;
				default:				//case(7):
					x = (float)i;
					y = (float)i;
					z = (float)j;
					upDirection.Points[0] = -1;
				}

				QbertBox_ptr box = QbertBox_ptr(new QbertBox(boxNameBefore, upDirection, face, x, y, z));
				InsertBox(Point3D(x, y, z), box);
			}
		}
	}

	SetQbert(qbertName, NULL);			//[todo] set the starting box for the Qbert;
	_iters.Init(_objects);

}

QbertModel::ModelObjectsIters DiamondQbertModel::GetObjectsToDraw() const
{
	return _iters;
}

void DiamondQbertModel::ReadInput(const QbertControler::InputData& inputdata)			//Deals with time and decides whether to move or not.
{

	_progress += (float)((inputdata.CertenTime - _lastTime) / _timeForMove);
	if (_progress > 1)
	{
		_objects.Qbert->_nowOn = _objects.Qbert->_afterOn;
		_objects.Qbert->_afterOn = GetBoxNeibhor(_objects.Qbert->_nowOn, inputdata.direction);
	}

	MakeEnemiesMove();

	while (_progress > 1)
		_progress--;

	_lastTime = inputdata.CertenTime;
}

void DiamondQbertModel::MakeEnemiesMove()
{
	for (std::list<QbertEnemyObj_ptr>::iterator iter = _objects.Enemies.begin(); iter != _objects.Enemies.end(); iter++)
	{
		(*iter)->Move(*this);
	}
}

DiamondQbertModel::~DiamondQbertModel(void)
{
}

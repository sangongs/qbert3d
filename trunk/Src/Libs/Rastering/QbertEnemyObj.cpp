#include "StdAfx.h"

#include "Point3D.h"
#include "QbertModel.h"

#include "QbertEnemyObj.h"

namespace BGComplete
{
	Direction QbertEnemyObj::WhereToMove()
	{
		if (!_isMoveLengthInit)
		{
			_moveLength = (static_cast<QbertModel*>(_model))->_enemiesMoveLengthes.find(_type)->second;
			_isMoveLengthInit = true;
		}

		if (!_isFuncInit)
		{
			_AIfunc = (static_cast<QbertModel*>(_model))->_AITypes.find(_type)->second;
			_isFuncInit = true;
		}

		return  _AIfunc(static_cast<QbertModel*>(_model));
	}

	bool QbertEnemyObj::IsQbertStillAlive()
	{
		if (LastBox == (static_cast<QbertModel*>(_model))->_objects.Qbert->NextBox && 
			NextBox == (static_cast<QbertModel*>(_model))->_objects.Qbert->LastBox)
			return false;
		
		if (((QbertModel*)_model)->_objects.Qbert->Progress > 0.3 && Progress > 0.8 && 
			NextBox == (static_cast<QbertModel*>(_model))->_objects.Qbert->LastBox)
			return false;

		return true;
	}
}

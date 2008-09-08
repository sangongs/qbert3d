#include "StdAfx.h"

#include "Point3D.h"
#include "QbertModel.h"

#include "QbertEnemyObj.h"

namespace BGComplete
{
	Direction QbertEnemyObj::WhereToMove()
	{
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

	QbertEnemyObj::~QbertEnemyObj(void) {}
}

#include "StdAfx.h"

#include "Point3D.h"
#include "QbertModel.h"

#include "QbertEnemyObj.h"

namespace BGComplete
{


const float QbertEnemyObj::_floatEquPrecision = 0.001f;
const float QbertEnemyObj::_point3DEquPrecision = 0.003f;
const float QbertEnemyObj::_killingDist = 0.2f;
std::map<std::string, VecOfAppearanceBox_ptr> QbertEnemyObj::_apperanceMap;

bool QbertEnemyObj::IsQbertStillAlive()
{
	return Center.IsEqual((static_cast<QbertModel*>(_model))->_objects->Qbert->Center, _killingDist);
}

VecOfAppearanceBox_ptr QbertEnemyObj::GetAppearanceBoxes()
{
	return _apperanceMap.find(_type)->second;
}

QbertEnemyObj::~QbertEnemyObj(void) {}

}	//namespace BGComplete

#include "StdAfx.h"

#include "Point3D.h"
#include "QbertModel.h"

#include "QbertEnemyObj.h"

namespace BGComplete
{

std::map<std::string, VecOfAppearanceBox_ptr> QbertEnemyObj::_apperanceMap;

bool QbertEnemyObj::IsQbertDies()
{
	return TestCollision((static_cast<QbertModel*>(_model))->_objects->Qbert);
}

VecOfAppearanceBox_ptr QbertEnemyObj::GetAppearanceBoxes()
{
	return _apperanceMap.find(_type)->second;
}

QbertEnemyObj::~QbertEnemyObj(void) {}

}	//namespace BGComplete

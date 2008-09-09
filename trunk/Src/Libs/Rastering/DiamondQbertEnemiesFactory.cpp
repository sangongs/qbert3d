#include "StdAfx.h"

#include "QbertEnemyObj.h"
#include "DiamondQbertModelEnemyBall.h"

#include "DiamondQbertEnemiesFactory.h"

namespace BGComplete
{

QbertEnemyObj_ptr DiamondQbertEnemiesFactory::GetNewEnemy (const std::string& type, const std::string& name, Model* model)
{
	if (type == "ball")
		return DiamondQbertModelEnemyBall_ptr(new DiamondQbertModelEnemyBall(name, model));

	return QbertEnemyObj_ptr();
}

}	//namespace BGComplete
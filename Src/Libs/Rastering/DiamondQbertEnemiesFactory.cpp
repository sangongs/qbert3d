#include "StdAfx.h"

#include "QbertEnemyObj.h"
#include "DiamondQbertModelEnemyBall.h"
#include "DiamondQbertModelEnemyDirect.h"

#include "DiamondQbertEnemiesFactory.h"

namespace BGComplete
{

QbertEnemyObj_ptr DiamondQbertEnemiesFactory::GetNewEnemy (const std::string& type, const std::string& name, Model* model, int score)
{
	if (type == "ball")
		return QbertEnemyObj_ptr(new DiamondQbertModelEnemyBall(name, model, score));
	if (type == "directEnemy")
		return QbertEnemyObj_ptr(new DiamondQbertModelEnemyDirect(name, model, score));

	return QbertEnemyObj_ptr();
}

}	//namespace BGComplete
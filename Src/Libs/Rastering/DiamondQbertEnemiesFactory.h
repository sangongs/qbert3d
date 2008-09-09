#pragma once

#include "boost/shared_ptr.hpp"

namespace BGComplete
{

class QbertEnemyObj;

class DiamondQbertEnemiesFactory
{
public:
	static boost::shared_ptr<QbertEnemyObj> GetNewEnemy (const std::string& type, const std::string& name, Model*);
};

}	//namespace BGComplete

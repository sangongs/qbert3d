#pragma once

#include <list>

#include "GameObject.h"

class Model
{
public:
	Model();
	virtual void TimeTick(unsigned deltaT) = 0;
	virtual void Move(unsigned right, unsigned left, unsigned forword, unsigned backword) = 0;
	virtual std::list<GameObject>* GetObjects() = 0;
};

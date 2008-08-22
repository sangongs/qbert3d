#pragma once

#include <list>

class Model
{
public:
	virtual void TimeTick(unsigned deltaT) = 0;
	virtual void Move(unsigned right, unsigned left, unsigned forword, unsigned backword) = 0;
	virtual std::list<GameObject>* GetObjects() = 0;
};

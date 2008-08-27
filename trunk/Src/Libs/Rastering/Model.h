#pragma once

#include <list>

#include "GameObject.h"

class Model
{
public:
	Model();
	virtual std::list<GameObject_ptr>* GetObjects() = 0;
};

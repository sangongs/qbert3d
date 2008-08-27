#pragma once

#include "Model.h"

class SimpleModel : public Model
{
private:
	std::list<GameObject_ptr> _gameObjects;

public:
	void TimeTick(unsigned deltaT);
	void Move(unsigned right, unsigned left, unsigned forword, unsigned backword);
	std::list<GameObject_ptr>* GetObjects();

	SimpleModel();
};

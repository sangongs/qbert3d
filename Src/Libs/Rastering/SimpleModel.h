#pragma once

#include "Model.h"

class SimpleModel : public Model
{
private:
	std::list<GameObject> _gameObjects;

public:
	void TimeTick(unsigned deltaT);
	void Move(unsigned right, unsigned left, unsigned forword, unsigned backword);
	std::list<GameObject>* GetObjects();

	SimpleModel();
};

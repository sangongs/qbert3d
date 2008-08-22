#pragma once

#include <list>

#include "GameObject.h"

class View
{
public:
	virtual void SetGameObjects(std::list<gameObject> *objList);
	virtual void Draw() = 0;
	virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate) = 0;
	virtual void ChangeResolution(unsigned width, unsigned height) = 0;
	virtual void Init(unsigned width, unsigned height) = 0;
};

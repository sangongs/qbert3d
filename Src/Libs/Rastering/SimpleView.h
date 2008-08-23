#pragma once

#include <map>

#include "View.h"
#include "DrawableObj.h"


class SimpleView : public View
{
private:
	float _x, _y, _z, _xRotate, _yRotate, _zRotate;
	std::map<std::string, DrawableObj_Ptr> _objects;
	std::list<GameObject> *_objList;

public:
	SimpleView();
	void SetGameObjects(std::list<GameObject> *objList);
	void Draw();
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate);
	void ChangeResolution(unsigned width, unsigned height);
	void Init(unsigned width, unsigned height);
};

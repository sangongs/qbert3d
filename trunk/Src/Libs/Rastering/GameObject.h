#pragma once

#include <string>

#include "boost/shared_ptr.hpp"

#include "DrawableObj.h"

class GameObject
{
public:
	float X, Y, Z, XRotate, YRotate, ZRotate;
	std::string Name;
	GameObject (const std::string& name ="", float x = 0, float y = 0, float z = 0, float xRotate = 0, float yRotate = 0, float zRotate = 0)
		: X(x), Y(y), Z(z), XRotate(xRotate), YRotate(yRotate), ZRotate(zRotate), Name(name) {}
	Point3D GetCoordinates() {return Point3D(X, Y, X);}
};

typedef boost::shared_ptr<GameObject> GameObject_ptr;

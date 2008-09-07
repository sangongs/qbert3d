#pragma once

#include <string>

#include "boost/shared_ptr.hpp"

class Point3D;
class Model;

class GameObject
{
protected:
	Model* _model;

public:
	Point3D Center;
	float XRotate, YRotate, ZRotate;
	std::string Name;
	GameObject (const std::string& name = "", float x = 0, float y = 0, float z = 0, float xRotate = 0, float yRotate = 0, float zRotate = 0)
		: Center(Point3D(x, y, z)), XRotate(xRotate), YRotate(yRotate), ZRotate(zRotate), Name(name) {}
	
	void SetModel(Model* model) {_model = model;}
};

typedef boost::shared_ptr<GameObject> GameObject_ptr;

#include "StdAfx.h"

#include <list>

#include "GameObject.h"

#include "SimpleModel.h"

SimpleModel::SimpleModel()
{
	GameObject obj;
	obj.X = 0;
	obj.Y = 40;
	obj.Z = -20;
	obj.XRotate = 0;
	obj.YRotate = 0;
	obj.ZRotate = 0;
	obj.Name = "object__2";

	_gameObjects.push_back(obj);
}

void SimpleModel::TimeTick(unsigned /*deltaT*/)
{

}

std::list<GameObject>* SimpleModel::GetObjects()
{
	return &_gameObjects;
}

void SimpleModel::Move(unsigned /*right*/, unsigned /*left*/, unsigned /*forword*/, unsigned /*backword*/)
{


}

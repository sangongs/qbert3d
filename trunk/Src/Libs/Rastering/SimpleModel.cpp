#include "StdAfx.h"

#include <list>

#include "GameObject.h"

#include "SimpleModel.h"

SimpleModel::SimpleModel()
{
	GameObject_ptr obj = boost::shared_ptr<GameObject>(new GameObject());
	obj->X = 0;
	obj->Y = 0;
	obj->Z = -1;
	obj->XRotate = 80;
	obj->YRotate = 0;
	obj->ZRotate = 0;
	obj->Name = "object__2";

	_gameObjects.push_back(obj);
}

void SimpleModel::TimeTick(unsigned /*deltaT*/)
{

}

std::list<GameObject_ptr>* SimpleModel::GetObjects()
{
	return &_gameObjects;
}

void SimpleModel::Move(unsigned /*right*/, unsigned /*left*/, unsigned /*forword*/, unsigned /*backword*/)
{


}

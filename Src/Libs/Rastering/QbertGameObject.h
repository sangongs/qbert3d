#pragma once

#include "boost/shared_ptr.hpp"
#include <Windows.h>

#include "GameObject.h"
#include "SimpleControler.h"

#include "QbertBox.h"


class QbertGameObject : public GameObject
{
protected:
	DWORD _moveLength;
	float _verticalSpeed, _horizontalSpeed, _freeFallAcceleration;

public:
	float Progress;
	QbertBox_ptr LastBox, NextBox;
	Point3D LastUpDirection, LastFaceDirection, NextUpDirection, NextFaceDirection, CurrentUpDirection, CurrentFaceDirection;
	bool IsMovingUp, IsChangingBox, IsMoving;
	Direction MovingDirection;

	QbertGameObject(const std::string& name ="", QbertBox_ptr box = QbertBox_ptr(), DWORD moveLength = 1000, float freeFallAcceleration = 10)
		: GameObject(name, 0, 0, 0, 0, 0, 0), LastBox(box), NextBox(box), Progress(0), _moveLength(moveLength) , IsMoving(false), _freeFallAcceleration(freeFallAcceleration)
	{
		SetMoveLength(moveLength, _freeFallAcceleration);
	}

	~QbertGameObject() {}

	Point3D GetRightDirection() {return LastFaceDirection.CrossProduct(LastUpDirection);}
	DWORD GetMoveLength() {return _moveLength;}
	float GetHorizontalSpeed() {return _horizontalSpeed;}
	float GetVerticalSpeed(){return _verticalSpeed;}
	float GetFreeAcceleration(){return _freeFallAcceleration;}
	void SetMoveLength(DWORD moveLength, float freeFallAcceleration);
};
typedef boost::shared_ptr<QbertGameObject> QbertGameObject_ptr;
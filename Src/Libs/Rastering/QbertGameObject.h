#pragma once

#include "boost/shared_ptr.hpp"
#include <Windows.h>

#include "GameObject.h"
#include "SimpleControler.h"

#include "QbertBox.h"


namespace BGComplete
{
	class QbertGameObject : public GameObject
	{
	protected:
		static const float _floatEquPrecision;
		static const float _point3DEquPrecision;
		static const float _killingDist;

		bool _isQbert;
		DWORD _moveLength;
		float _verticalSpeed, _horizontalSpeed, _freeFallAcceleration;

	public:
		float Progress;
		QbertBox_ptr LastBox, NextBox;
		Math::Point3D LastUpDirection, LastFaceDirection, NextUpDirection, NextFaceDirection, CurrentUpDirection, CurrentFaceDirection;
		bool IsMovingUp, IsChangingBox, IsMoving, IsDying, DyingProgress;
		static const DWORD DyingDuration;
		Direction MovingDirection;

		QbertGameObject(const std::string& name ="", Model* model = NULL, QbertBox_ptr box = QbertBox_ptr(), DWORD moveLength = 1000, float freeFallAcceleration = 10, bool isQbert = true)
			: GameObject(name, model, 0, 0, 0, 0, 0, 0), LastBox(box), NextBox(box), Progress(0), _isQbert(isQbert), _moveLength(moveLength) , IsMoving(false),
				_freeFallAcceleration(freeFallAcceleration), IsDying(false)
		{
			SetMoveLength(moveLength, _freeFallAcceleration);
		}

		~QbertGameObject() {}

		Math::Point3D GetRightDirection() {return LastFaceDirection.CrossProduct(LastUpDirection);}
		Math::Point3D GetCurrentRightDirection() {return CurrentFaceDirection.CrossProduct(CurrentUpDirection);}
		DWORD GetMoveLength() {return _moveLength;}
		float GetHorizontalSpeed() {return _horizontalSpeed;}
		float GetVerticalSpeed(){return _verticalSpeed;}
		float GetFreeAcceleration(){return _freeFallAcceleration;}
		void SetMoveLength(DWORD moveLength, float freeFallAcceleration);
		bool IsQbert() {return _isQbert;}
		bool TestCollision(boost::shared_ptr<QbertGameObject>);
	};
	typedef boost::shared_ptr<QbertGameObject> QbertGameObject_ptr;
}

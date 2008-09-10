#include "StdAfx.h"

#include "Point3D.h"

#include "QbertGameObject.h"

namespace BGComplete
{

	const float QbertGameObject::_floatEquPrecision = 0.001f;
	const float QbertGameObject::_point3DEquPrecision = 0.003f;
	const float QbertGameObject::_killingDist = 0.2f;
	const DWORD QbertGameObject::DyingDuration = 750;

	void QbertGameObject::SetMoveLength(DWORD moveLength, float freeFallAcceleration)
	{
		_freeFallAcceleration = freeFallAcceleration;
		_moveLength = moveLength;
		_horizontalSpeed = 1;

		_verticalSpeed = std::abs(1.0f / _horizontalSpeed - 0.5f * _freeFallAcceleration * _horizontalSpeed);
	}


	bool QbertGameObject::TestCollision(QbertGameObject_ptr obj)
	{
		return Center.IsEqual(obj->Center, _killingDist);
	}
}

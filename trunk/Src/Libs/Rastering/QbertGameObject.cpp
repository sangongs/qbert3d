#include "StdAfx.h"

#include "Point3D.h"

#include "QbertGameObject.h"

void QbertGameObject::SetMoveLength(DWORD moveLength, float freeFallAcceleration)
{
	_freeFallAcceleration = freeFallAcceleration;
	_moveLength = moveLength;
	_horizontalSpeed = 1;

	_verticalSpeed = std::abs(1.0f / _horizontalSpeed - 0.5f * _freeFallAcceleration * _horizontalSpeed);
}
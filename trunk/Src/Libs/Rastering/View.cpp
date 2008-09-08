#include "stdafx.h"

#include "sdl/SDL_opengl.h"

#include "Math.h"
#include "Point3D.h"
#include "DrawableObj.h"

#include "View.h"

namespace BGComplete
{
	const float View::_floatEquPrecision = 0.001f;
	const float View::_point3DEquPrecision = 0.003f;

	void View::ChangeCoordinateSystem(std::pair<Math::Point3D, Math::Point3D>& from, std::pair<Math::Point3D, Math::Point3D>& to, bool validate)
	{
		from.first.Normalize();
		from.second.Normalize();
		to.first.Normalize();
		to.second.Normalize();

		if (validate)
		{
			float firstScalar = from.first.ScalarProduct(from.second), secondScalar = to.first.ScalarProduct(to.second);

			if (firstScalar == -1 || secondScalar == -1 || std::abs(firstScalar - secondScalar) > _floatEquPrecision)
				throw std::exception("Incompatible set of coordinate systems, can't match them (in function View::ChangeSystemCoordinate()");

			if (firstScalar)
			{
				from.second = (from.first.CrossProduct(from.second)).CrossProduct(from.first);
				to.second = (to.first.CrossProduct(to.second)).CrossProduct(to.first);
			}
		}

		Math::Point3D rotationDirection = 0.5 * (from.first + to.first);
		if (rotationDirection.IsEqual(Math::Point3D::Zero, _point3DEquPrecision))
			rotationDirection = to.second;

		Math::Point3D translatedSecondVector = (2 * (rotationDirection.ScalarProduct(from.second) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
			from.second).Normalize();

		float rotationAngle = Math::Dacos(translatedSecondVector.ScalarProduct(to.second));
		Math::Point3D normalVector = translatedSecondVector.CrossProduct(to.second);
		if (!normalVector.IsEqual(Math::Point3D::Zero, _point3DEquPrecision) && !normalVector.Normalize().IsEqual(to.first, _point3DEquPrecision))
				rotationAngle *= -1;

		glRotatef(rotationAngle, to.first.X(), to.first.Y(), to.first.Z());
		glRotatef(180, rotationDirection.X(), rotationDirection.Y(), rotationDirection.Z());
	}
}

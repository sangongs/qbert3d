#include "stdafx.h"

#include "sdl/SDL_opengl.h"

#include "Point3D.h"
#include "DrawableObj.h"

#include "View.h"



const float View::floatEquPrecision = 0.001f;
const float View::point3DEquPrecision = 0.003f;

void View::ChangeCoordinateSystem(std::pair<Point3D,Point3D>& from, std::pair<Point3D,Point3D>& to, bool validate)
{
	from.first.Normalize();
	from.second.Normalize();
	to.first.Normalize();
	to.second.Normalize();

	if (validate)
	{
		float firstScalar = from.first.ScalarProduct(from.second), secondScalar = to.first.ScalarProduct(to.second);

		if (firstScalar == -1 || secondScalar == -1 || std::abs(firstScalar - secondScalar) > floatEquPrecision)
			throw std::exception("Incompatible set of coordinate systems, can't match them (in function View::ChangeSystemCoordinate()");

		if (firstScalar)
		{
			from.second = (from.first.CrossProduct(from.second)).CrossProduct(from.first);
			to.second = (to.first.CrossProduct(to.second)).CrossProduct(to.first);
		}
	}

	Point3D rotationDirection = 0.5 * (from.first + to.first);
	if (rotationDirection.IsEqual(Point3D::Zero, point3DEquPrecision))
		rotationDirection = to.second;

	Point3D translatedSecondVector = (2 * (rotationDirection.ScalarProduct(from.second) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
		from.second).Normalize();

	float rotationAngle = Dacos(translatedSecondVector.ScalarProduct(to.second));
	Point3D normalVector = translatedSecondVector.CrossProduct(to.second);
	if (!normalVector.IsEqual(Point3D::Zero, point3DEquPrecision) && !normalVector.Normalize().IsEqual(to.first, point3DEquPrecision))
			rotationAngle *= -1;

	glRotatef(rotationAngle, to.first.X(), to.first.Y(), to.first.Z());
	glRotatef(180, rotationDirection.X(), rotationDirection.Y(), rotationDirection.Z());
}

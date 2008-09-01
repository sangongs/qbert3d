#include "stdafx.h"

#include "sdl/SDL_opengl.h"

#include "DrawableObj.h"

#include "View.h"

void View::ChangeCoordinateSystem(std::pair<Point3D,Point3D>& from, std::pair<Point3D,Point3D>& to, bool validate)
{
	from.first.Normalize();
	from.second.Normalize();
	to.first.Normalize();
	to.second.Normalize();

	float firstScalar = from.first.ScalarProduct(from.second);
	if (validate)
	{
		float secondScalar = to.first.ScalarProduct(to.second);
		if (firstScalar == -1 || secondScalar == -1 || std::abs(firstScalar - secondScalar) > 0.001) //[todo] remove this 0.001 value from here, must be const somewhere else.
			throw std::exception("Incompatible set of coordinate systems, can't match them (in function View::ChangeSystemCoordinate()");
	}

	if (firstScalar)
	{
		Point3D rightDirection = from.first.CrossProduct(from.second);
		from.second = rightDirection.CrossProduct(from.first);

		rightDirection = to.first.CrossProduct(to.second);
		to.second = rightDirection.CrossProduct(to.first);
	}

	//[todo] dont do second check if validate

	Point3D rotationDirection = 0.5 * (from.first + to.first);
	if ((rotationDirection.Points[0] == 0) && (rotationDirection.Points[1] == 0) && (rotationDirection.Points[2] == 0))
		rotationDirection = to.second;

	Point3D translatedSecondVector = (2 * (rotationDirection.ScalarProduct(from.second) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
		from.second).Normalize();

	float rotationAngle = Dacos(translatedSecondVector.ScalarProduct(to.second));
	Point3D normalVector = translatedSecondVector.CrossProduct(to.second);
	if (normalVector.X() != 0 || normalVector.Y() != 0 || normalVector.Z() != 0)
	{
		normalVector.Normalize();

		//[todo] insert this code into Point3D checkEquilty(float precision), also please remove 0.003 onto somewhere else.
		if (std::abs(normalVector.X() - to.first.X()) > 0.003 ||
			std::abs(normalVector.Y() - to.first.Y()) > 0.003 ||
			std::abs(normalVector.Z() - to.first.Z()) > 0.003)
			rotationAngle *= -1;
	}

	glRotatef(rotationAngle, to.first.X(), to.first.Y(), to.first.Z()); //[todo] check if glRotatefv instead
	glRotatef(180, rotationDirection.X(), rotationDirection.Y(), rotationDirection.Z()); //here too
}

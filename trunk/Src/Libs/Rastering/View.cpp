#include "stdafx.h"

#include "sdl/SDL_opengl.h"
#include "DrawableObj.h"

#include "View.h"


void View::ChangeSystemCoordinate(std::pair<Point3D,Point3D>& from, std::pair<Point3D,Point3D>& to, bool toCheck = true)
{

	from.first.Normalize();
	from.second.Normalize();
	to.first.Normalize();
	to.second.Normalize();

	float temp = from.first.ScalarProduct(from.second);
	if (toCheck)
	{
		float temp2 = to.first.ScalarProduct(to.second);
		if (temp == -1 || temp2 == -1 || std::abs(temp - temp2) > 0.001)
			throw std::exception("incompatible Systems of coordinates to match between them (in function View::ChangeSystemCoordinate()");
	}

	if (temp != 0)
	{
		Point3D right = from.first.CrossProduct(from.second);
		from.second = right.CrossProduct(from.first);

		right = to.first.CrossProduct(to.second);
		to.second = right.CrossProduct(to.first);
	}


	Point3D rotationDirection = 0.5 * (from.first + to.first);
	if ((rotationDirection.Points[0] == 0) && (rotationDirection.Points[1] == 0) && (rotationDirection.Points[2] == 0))
		rotationDirection = to.second;

	Point3D tempSecondDirection = (2 * (rotationDirection.ScalarProduct(from.second) / rotationDirection.ScalarProduct(rotationDirection)) * rotationDirection - 
		from.second).Normalize();



	float rotationAngle = Dacos(tempSecondDirection.ScalarProduct(to.second));
	Point3D normalVector = tempSecondDirection.CrossProduct(to.second);
	if (normalVector.X() != 0 || normalVector.Y() != 0 || normalVector.Z() != 0)
	{
		normalVector.Normalize();
		if (std::abs(normalVector.X() - to.first.X()) > 0.003 ||
			std::abs(normalVector.Y() - to.first.Y()) > 0.003 ||
			std::abs(normalVector.Z() - to.first.Z()) > 0.003)
			rotationAngle *= -1;
	}

	glRotatef(rotationAngle, to.first.X(), to.first.Y(), to.first.Z());
	glRotatef(180, rotationDirection.X(), rotationDirection.Y(),rotationDirection.Z());
}
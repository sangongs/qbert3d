#include "StdAfx.h"

#include "Point3D.h"

namespace BGComplete
{
	namespace Math
	{
		const Point3D Point3D::Zero(0, 0, 0);

		Point3D::Point3D(float x, float y, float z)
		{
			Points[0] = x;
			Points[1] = y;
			Points[2] = z;
		}

		Point3D::Point3D(const Point3D& point)
		{
			Points[0] = point.Points[0];
			Points[1] = point.Points[1];
			Points[2] = point.Points[2];
		}

		void Point3D::LoadFromArguments(const std::string& arguments)
		{
			if (sscanf(arguments.c_str(), "%f %f %f", Points, Points + 1, Points + 2) != 3)
				throw std::exception("Couldn't read line into Point3D");
		}

		Point3D& Point3D::operator= (const Point3D& point)
		{
			Points[0] = point.Points[0];
			Points[1] = point.Points[1];
			Points[2] = point.Points[2];
			return *this;
		}

		Point3D Point3D::operator- () const
		{
			return Point3D(-Points[0], -Points[1], -Points[2]);
		}

		Point3D& Point3D::operator+= (const Point3D& point)
		{
			Points[0] += point.Points[0];
			Points[1] += point.Points[1];
			Points[2] += point.Points[2];
			return *this;
		}

		Point3D& Point3D::operator-= (const Point3D& point)
		{
			Points[0] -= point.Points[0];
			Points[1] -= point.Points[1];
			Points[2] -= point.Points[2];
			return *this;
		}

		Point3D& Point3D::operator*= (float factor)
		{
			Points[0] *= factor;
			Points[1] *= factor;
			Points[2] *= factor;
			return *this;
		}

		Point3D& Point3D::operator/= (float factor)
		{
			Points[0] /= factor;
			Points[1] /= factor;
			Points[2] /= factor;
			return *this;
		}

		Point3D Point3D::operator+ (const Point3D& point) const
		{
			return Point3D(Points[0] + point.Points[0],
				Points[1] + point.Points[1],
				Points[2] + point.Points[2]);
		}

		Point3D Point3D::operator- (const Point3D& point) const
		{
			return Point3D(Points[0] - point.Points[0],
				Points[1] - point.Points[1],
				Points[2] - point.Points[2]);
		}

		Point3D Point3D::operator* (float factor) const
		{
			return Point3D(Points[0] * factor, Points[1] * factor, Points[2] * factor);
		}

		Point3D Point3D::operator/ (float factor) const
		{
			return Point3D(Points[0] / factor, Points[1] / factor, Points[2] / factor);
		}

		bool Point3D::operator< (const Point3D& point) const   //for map compare.
		{
			if (Points[0] < point.Points[0])
				return true;
			else if (Points[0] > point.Points[0])
				return false;


			if (Points[1] < point.Points[1])
				return true;
			else if (Points[1] > point.Points[1])
				return false;


			if (Points[2] < point.Points[2])
				return true;
			else if (Points[2] > point.Points[2])
				return false;

			return false;
		}

		bool Point3D::IsEqual (const Point3D& point, float precition ) const
		{
			return (std::abs(X() - point.X()) <= precition &&
				std::abs(Y() - point.Y()) <= precition &&
				std::abs(Z() - point.Z()) <= precition);
		}

		Point3D& Point3D::Normalize()
		{
			float length = sqrt(Points[0]*Points[0] + Points[1]*Points[1] + Points[2]*Points[2]);

			if (length == 0)
				throw std::exception("can't divide by '0' (in function Point3D::Normalize())"); //can't devide by 0

			Points[0] /= length;
			Points[1] /= length;
			Points[2] /= length;

			return *this;
		}

		Point3D Point3D::CrossProduct (Point3D& point) const
		{
			return Point3D(Y() * point.Z() - Z() * point.Y(), Z() * point.X() - X() * point.Z(), X() * point.Y() - Y() * point.X());
		}

		float Point3D::ScalarProduct (Point3D& point) const
		{
			return (Points[0] * point.Points[0] + Points[1] * point.Points[1] + Points[2] * point.Points[2]);
		}

		Point3D Point3D::CrossProduct (float x, float y, float z) const
		{
			return Point3D(Y() * z - Z() * y, Z() * x - X() * z, X() * y - Y() * x);
		}

		float Point3D::ScalarProduct (float x, float y, float z) const
		{
			return (Points[0] * x + Points[1] * y + Points[2] * z);
		}

		Point3D operator* (float factor, const Point3D& point)
		{
			return Point3D(point.Points[0] * factor, point.Points[1] * factor, point.Points[2] * factor);
		}
	}
}

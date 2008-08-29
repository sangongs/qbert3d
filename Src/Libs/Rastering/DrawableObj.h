#pragma once

#include <math.h>

class Point3D
{
public:
	Point3D(float x = 0, float y = 0, float z = 0)
	{
		Points[0] = x;
		Points[1] = y;
		Points[2] = z;
	}

	Point3D(const std::string& arguments)
	{
		LoadFromArguments(arguments);
	}

	Point3D(const Point3D& point)
	{
		Points[0] = point.Points[0];
		Points[1] = point.Points[1];
		Points[2] = point.Points[2];
	}

	Point3D& operator= (const Point3D& point)
	{
		Points[0] = point.Points[0];
		Points[1] = point.Points[1];
		Points[2] = point.Points[2];
		return *this;
	}

	Point3D operator- ()
	{
		return Point3D(-Points[0], -Points[1], -Points[2]);
	}

	Point3D& operator+= (const Point3D& point)
	{
		Points[0] += point.Points[0];
		Points[1] += point.Points[1];
		Points[2] += point.Points[2];
		return *this;
	}

	Point3D& operator-= (const Point3D& point)
	{
		Points[0] -= point.Points[0];
		Points[1] -= point.Points[1];
		Points[2] -= point.Points[2];
		return *this;
	}

	Point3D operator+ (const Point3D& point)
	{
		return Point3D(Points[0] + point.Points[0],
			Points[1] + point.Points[1],
			Points[2] + point.Points[2]);
	}

	Point3D operator- (const Point3D& point)
	{
		return Point3D(Points[0] - point.Points[0],
			Points[1] - point.Points[1],
			Points[2] - point.Points[2]);
	}

	Point3D operator* (float factor)
	{
		return Point3D(Points[0] * factor, Points[1] * factor, Points[2] * factor);
	}

	void LoadFromArguments(const std::string& arguments)
	{
		if (sscanf(arguments.c_str(), "%f %f %f", Points, Points + 1, Points + 2) != 3)
			throw std::exception("Couldn't read line into Point3D");
	}

	bool operator< (const Point3D& point) const   //for map compreation.
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

	bool operator == (const Point3D point) const
	{
		return (Points[0] == point.Points[0] && Points[1] == point.Points[1] && Points[2] == point.Points[2]);
	}

	Point3D& Normalize()
	{
		float length = sqrt(Points[0]*Points[0] + Points[1]*Points[1] + Points[2]*Points[2]);

		if (length == 0)
			throw std::exception("can't divide by '0' (in function Point3D::Normalize())"); //can't devide by 0

		Points[0] /= length;
		Points[1] /= length;
		Points[2] /= length;

		return *this;
	}

	float& X()
	{
		return Points[0];
	}

	float& Y()
	{
		return Points[1];
	}

	float& Z()
	{
		return Points[2];
	}


	Point3D CrossProduct (Point3D& point)
	{
		return Point3D(Y() * point.Z() - Z() * point.Y(), Z() * point.X() - X() * point.Z(), X() * point.Y() - Y() * point.X());
	}

	float ScalarProduct (Point3D& point)
	{
		return (Points[0] * point.Points[0] + Points[1] * point.Points[1] + Points[2] * point.Points[2]);
	}

	float Points[3];
};

class Point2D
{
public:
	Point2D(const std::string& argument)
	{
		if (sscanf(argument.c_str(), "%f %f", Points, Points + 1) != 2)
			throw std::exception("Couldn't read line into Point2D");
	}

	Point2D() {}

	float Points[2];
};


class DrawableObj
{
private:
	int _listNum;
	float _yDistFromFloor;

public:
	DrawableObj::DrawableObj(const std::string& directory, const std::string &fileName, float scale, float rotateX, float rotateY, float rotateZ);
	~DrawableObj();
	
	void Draw(float rotateX, float rotateY, float rotateZ, float scale, bool onYPlane);
};

typedef boost::shared_ptr<DrawableObj> DrawableObj_Ptr;


Point3D operator* (float factor, const Point3D&);

float DSin(float degrees);

float DCos(float degrees);

float Modulu(float inp);



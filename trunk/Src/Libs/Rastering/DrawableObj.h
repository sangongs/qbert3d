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

	Point3D& operator+= (const Point3D& point)
	{
		Points[0] += point.Points[0];
		Points[1] += point.Points[1];
		Points[2] += point.Points[2];
		return *this;
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

	void Normalize()
	{
		float length = sqrt(Points[0]*Points[0] + Points[1]*Points[1] + Points[2]*Points[2]);

		if (length == 0)
			return; //can't devide by 0

		Points[0] /= length;
		Points[1] /= length;
		Points[2] /= length;
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
	
	void Draw(float rotateX, float rotateY, float rotateZ, float scale);
};

typedef boost::shared_ptr<DrawableObj> DrawableObj_Ptr;

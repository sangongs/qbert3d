#pragma once

#include <string>

class Point3D
{
public:
	Point3D(float x = 0, float y = 0, float z = 0);
	Point3D(const Point3D& point);
	Point3D& operator= (const Point3D& point);
	Point3D(const std::string& arguments) {LoadFromArguments(arguments);}
	void LoadFromArguments(const std::string& arguments);

	~Point3D() {}

	Point3D operator- ();
	Point3D& operator+= (const Point3D& point);
	Point3D& operator-= (const Point3D& point);
	Point3D operator+ (const Point3D& point);
	Point3D operator- (const Point3D& point);
	Point3D operator* (float factor);

	bool operator == (const Point3D point) const {return (X() == point.X() && Y() == point.Y() && Z() == point.Z());}
	bool operator != (const Point3D point) const {return (X() != point.X() || Y() != point.Y() || Z() != point.Z());}
	bool IsEqual (const Point3D& point, float precition = 0);
	bool operator< (const Point3D& point) const;   //for map compare.


	Point3D& Normalize();

	float& X() {return Points[0];}
	float& Y() {return Points[1];}
	float& Z() {return Points[2];}

	float X() const {return Points[0];}
	float Y() const {return Points[1];}
	float Z() const {return Points[2];}


	Point3D CrossProduct (Point3D& point);
	float ScalarProduct (Point3D& point);

	float Points[3];

	static const Point3D Zero;
};


Point3D operator* (float factor, const Point3D&);
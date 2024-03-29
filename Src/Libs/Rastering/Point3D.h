#pragma once

#include <string>

namespace BGComplete
{
	namespace Math
	{
		class Point3D
		{
		public:
			Point3D(float x = 0, float y = 0, float z = 0);
			Point3D(const Point3D& point);
			Point3D& operator= (const Point3D& point);
			Point3D(const std::string& arguments) {LoadFromArguments(arguments);}
			void LoadFromArguments(const std::string& arguments);

			~Point3D() {}

			Point3D operator- () const;
			Point3D& operator+= (const Point3D& point);
			Point3D& operator-= (const Point3D& point);
			Point3D& operator*= (float factor);
			Point3D& operator/= (float factor);
			Point3D operator+ (const Point3D& point) const;
			Point3D operator- (const Point3D& point) const;
			Point3D operator* (float factor) const;
			Point3D operator/ (float factor) const;

			bool operator == (const Point3D point) const {return (X() == point.X() && Y() == point.Y() && Z() == point.Z());}
			bool operator != (const Point3D point) const {return (X() != point.X() || Y() != point.Y() || Z() != point.Z());}
			bool IsEqual (const Point3D& point, float precition = 0) const;
			bool operator< (const Point3D& point) const;   //for map compare.


			Point3D& Normalize();

			float& X() {return Points[0];}
			float& Y() {return Points[1];}
			float& Z() {return Points[2];}
			float& operator[] (int index) {return Points[index];}

			float X() const {return Points[0];}
			float Y() const {return Points[1];}
			float Z() const {return Points[2];}
			float operator[] (int index) const {return Points[index];}



			Point3D CrossProduct (Point3D& point) const;
			float ScalarProduct (Point3D& point) const;

			Point3D CrossProduct (float x, float y, float z) const;
			float ScalarProduct (float x, float y, float z) const;

			float Points[3];

			static const Point3D Zero;
		};


		Point3D operator* (float factor, const Point3D&);
	}
}

#pragma once

#include <list>

#include "GameObject.h"

namespace BGComplete
{
	class Point3D;

	class View
	{
	private:
		const static float _floatEquPrecision;			//Defined in the View.cpp file
		static const float _point3DEquPrecision;		//Defined in the View.cpp file

	public:
		virtual void SetGameObjects(std::list<GameObject_ptr> *objList) = 0; //[todo] remove this line?
		virtual void Draw(bool clearScreen = true) = 0;
		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate) = 0;
		virtual void ChangeResolution(unsigned width, unsigned height) = 0;
		virtual void Init(unsigned width, unsigned height) = 0;
		void ChangeCoordinateSystem (std::pair<Math::Point3D, Math::Point3D> from, std::pair<Math::Point3D, Math::Point3D> to, bool validate = true);
	};
}

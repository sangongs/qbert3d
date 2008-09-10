#pragma once

#include <string>
#include "boost/shared_ptr.hpp"

#include "Point3D.h"

namespace BGComplete
{
	class Math::Point3D;
	class Model;

	class GameObject
	{
	protected:
		Model* _model;

	public:
		Math::Point3D Center;
		float XRotate, YRotate, ZRotate, Scale;
		std::string Name;
		GameObject (const std::string& name = "", Model* model = NULL, float x = 0, float y = 0, float z = 0, float xRotate = 0, float yRotate = 0, float zRotate = 0, float scale = 1)
			: Center(Math::Point3D(x, y, z)), XRotate(xRotate), YRotate(yRotate), ZRotate(zRotate), Name(name), _model(model), Scale(scale) {}
		
		void SetModel(Model* model) {_model = model;}
	};

	typedef boost::shared_ptr<GameObject> GameObject_ptr;
}

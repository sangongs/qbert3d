#pragma once

#include "boost/shared_ptr.hpp"
#include "GameObject.h"

namespace BGComplete
{
	class QbertBox : public GameObject
	{
		friend class QbertModel;

		int _face;
		bool _isVisited;
		bool _isOnPerimeter;
	public:
		QbertBox(const std::string& name, int face, bool isPerimeter,
			float x, float y, float z, float xRotate = 0, float yRotate = 0, float zRotate = 0)
			: GameObject(name, x, y, z, xRotate, yRotate, zRotate), _face(face), _isVisited(false), _isOnPerimeter(isPerimeter) {}

		bool IsOnPerimeter() {return _isOnPerimeter;}
	};
	typedef boost::shared_ptr<QbertBox> QbertBox_ptr;
}

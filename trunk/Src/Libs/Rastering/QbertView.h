#pragma once

#include <map>

#include "DrawableObj.h"

#include "QbertModel.h"

namespace BGComplete
{
	class QbertView
	{
	public:
		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, bool viewChange) = 0;
		virtual void Draw(QbertModel::ModelObjects& modelObjects, bool clear, unsigned startX, unsigned startY, unsigned width, unsigned height) = 0;
		virtual void Init() = 0;
	};

	typedef boost::shared_ptr<QbertView> QbertView_Ptr;
}

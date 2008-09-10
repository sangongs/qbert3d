#pragma once

#include <map>

#include "View.h"
#include "QbertModel.h"

namespace BGComplete
{
	class QbertView : public View
	{
	public:
		virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey) = 0;
		virtual void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects) = 0;
		virtual void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height) = 0;
		virtual void Init() = 0;
	};

	typedef boost::shared_ptr<QbertView> QbertView_Ptr;
}

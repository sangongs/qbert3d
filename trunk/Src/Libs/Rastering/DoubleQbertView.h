#pragma once

#include "QbertView.h"

namespace BGComplete
{

class DoubleQbertView : public QbertView
{
	private:
		QbertView_Ptr _views[2];
		float _ratio;
		bool _state;

	protected:
		void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);

	public:
		DoubleQbertView(QbertView_Ptr view1, QbertView_Ptr view2, float ratio);
		
		void Draw(QbertModel::ModelObjects& modelObjects, bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		void Init();
};

}
#pragma once

#include "QbertView.h"

namespace BGComplete
{

class LayeredQbertView : QbertView
{
private:
	std::list<QbertView_Ptr> _views;

protected:
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);

public:
	LayeredQbertView(std::list<QbertView_Ptr>& inp);

	void Draw(QbertModel::ModelObjects& modelObjects, bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}

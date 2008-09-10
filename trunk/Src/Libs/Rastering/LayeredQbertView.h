#pragma once

#include "QbertView.h"

namespace BGComplete
{

class LayeredQbertView : QbertView
{
private:
	std::list<QbertView_Ptr> _qbertViews;
	std::list<View_Ptr> _simpleViews;

protected:
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);

public:
	LayeredQbertView(std::list<QbertView_Ptr>& qbertViews, std::list<View_Ptr>& simpleViews);

	void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}

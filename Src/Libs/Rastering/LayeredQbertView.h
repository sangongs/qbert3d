#pragma once

#include "Point2D.h"

#include "QbertView.h"

namespace BGComplete
{

class LayeredQbertView : QbertView
{
public:
	typedef std::pair<Math::Point2D, Math::Point2D> QuadCoords;
	typedef std::pair<View_Ptr, QuadCoords> CoordinatedSimpleView;
	typedef std::pair<QbertView_Ptr, QuadCoords> CoordinatedQbertView;

	LayeredQbertView(std::list<CoordinatedQbertView>& qbertViews, std::list<CoordinatedSimpleView>& simpleViews);

	void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
private:
	std::list<CoordinatedQbertView> _qbertViews;
	std::list<CoordinatedSimpleView> _simpleViews;

protected:
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
};

}

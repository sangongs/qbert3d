#pragma once

#include "QbertView.h"

namespace BGComplete
{

class ScoreView : public QbertView
{
private:
	unsigned _displayedScore;
	QbertModel::ModelObjects_Ptr _modelObjects;
	FontView _font;

public:
	ScoreView();
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}
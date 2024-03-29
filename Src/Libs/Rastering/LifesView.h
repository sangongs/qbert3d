#pragma once

#include "DrawableObj.h"

#include "QbertView.h"

namespace BGComplete
{
	class LifesView : public QbertView
	{
	private:
		int _currentLifes;
		DrawableObj_Ptr _heart;

	public:
		LifesView();
		void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
		void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
		void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		void Init();
	};
}
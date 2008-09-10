#pragma once

#include "QbertView.h"

namespace BGComplete
{
	class LifesView : public QbertView
	{
	private:
		FontView _lifesFont;
		int _currentLifes;

	public:
		LifesView();
		void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
		void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
		void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		void Init();
	};
}
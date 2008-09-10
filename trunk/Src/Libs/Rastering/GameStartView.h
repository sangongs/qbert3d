#pragma once

#include "QbertView.h"

namespace BGComplete
{

	class GameStartView : public QbertView
	{
	private:
		QbertModel::ModelObjects_Ptr _modelObjects;
		FontView _gameStartFont, _pressNFont, _bgComplete;

	public:
		GameStartView();
		void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
		void SetUpDrawModel(QbertModel::ModelObjects_Ptr modelObjects);
		void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
		void Init();
	};

}
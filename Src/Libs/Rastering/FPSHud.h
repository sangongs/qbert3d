#pragma once

#include "View.h"

namespace BGComplete
{

class FontView;

class FPSHud : public View
{
private:
	FontView _drawer;
	DWORD _lastTime;
	DWORD _timeAccumulator;
	unsigned _frameAccumulator;

public:
	FPSHud(const std::string& font, unsigned fontSize);
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}
#pragma once

namespace BGComplete
{

class ControlKeysView : public View
{
private:
	bool _showState;
	FontView  _linkText;
	std::vector<FontView> _instructionsText;

public:
	ControlKeysView();
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}
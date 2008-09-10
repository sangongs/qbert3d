#pragma once

namespace BGComplete
{

class View
{
public:
	virtual void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey) = 0;
	virtual void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height) = 0;
	virtual void Init() = 0;
};

typedef boost::shared_ptr<View> View_Ptr;

}
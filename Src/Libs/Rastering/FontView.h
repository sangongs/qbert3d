#pragma once

#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "View.h"

namespace BGComplete
{

class FontView : public View
{
private:
	TTF_Font *_font;
	GLuint _texture;
	float _quadPoints[12];

public:
	FontView(const std::string& fileName, unsigned fontSize);
	~FontView();
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	void SetText(const std::string& text, unsigned char r, unsigned char g, unsigned char b, float* points);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}
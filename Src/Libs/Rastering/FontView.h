#pragma once

#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "Point2D.h"

#include "View.h"

namespace BGComplete
{

class FontView : public View
{
private:
	class FontEncapsulator
	{
	public:
		TTF_Font *Font;
		
		FontEncapsulator(const std::string& fileName, unsigned fontSize) : Font(TTF_OpenFont(fileName.c_str(), fontSize))
		{
			if (!Font)
				throw std::exception("Error loading font");
		}
		~FontEncapsulator() {TTF_CloseFont(Font);}
	};

	typedef boost::shared_ptr<FontEncapsulator> FontEncapsulator_Ptr;

	FontEncapsulator_Ptr _font;
	GLuint _texture;
	bool _fitView;
	float _quadPoints[12];
	Math::Point2D _clip;

public:
	FontView(const std::string& fileName, unsigned fontSize);
	~FontView();
	void CameraMove(float deltaX, float deltaY, float deltaZ, float xRotate, float yRotate, float zRotate, char viewKey);
	void SetText(const std::string& text, unsigned char r, unsigned char g, unsigned char b, float* points);
	void Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height);
	void Init();
};

}
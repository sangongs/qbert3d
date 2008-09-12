#include "StdAfx.h"

#include <SDL\SDL.h>
#include <SDL\SDL_opengl.h>
#include <SDL\SDL_TTF.h>

#include "FontView.h"

#include "ControlKeysView.h"

namespace BGComplete
{
	ControlKeysView::ControlKeysView() : _showState(false), _linkText("FONTS\\CALIBRI.TTF", 28)
	{}

	void ControlKeysView::CameraMove(float /*deltaX*/, float /*deltaY*/, float /*deltaZ*/, float /*xRotate*/, float /*yRotate*/, float /*zRotate*/, char viewKey)
	{
		if (viewKey == '/')
			_showState = !_showState;
	}

	void ControlKeysView::Draw(bool clearAndSwap, unsigned startX, unsigned startY, unsigned width, unsigned height)
	{
		if (_showState)
		{		
			_instructionsText[0].Draw(clearAndSwap, startX + (unsigned)(width * 0.2), startY + (unsigned)(height * 0.77), (unsigned)(width * 0.2), (unsigned)(height * 0.08));
			_instructionsText[1].Draw(clearAndSwap, startX + (unsigned)(width * 0.2), startY + (unsigned)(height * (0.7 - 0.02 - 0.03 * 1)), (unsigned)(width * 0.65), (unsigned)(height * 0.03)); //arcade style baby
			_instructionsText[4].Draw(clearAndSwap, startX + (unsigned)(width * 0.2), startY + (unsigned)(height * (0.7 - 0.02 - 0.03 * 4)), (unsigned)(width * 0.375), (unsigned)(height * 0.03)); //arcade style baby

			for (int l = 2; l < 4; l++)
				_instructionsText[l].Draw(clearAndSwap, startX + (unsigned)(width * 0.2), startY + (unsigned)(height * (0.7 - 0.02 - 0.03 * l)), (unsigned)(width * 0.3), (unsigned)(height * 0.03));


		}
		else
			_linkText.Draw(clearAndSwap, startX + (unsigned)(width * 0.8), startY, (unsigned)(width * 0.2), (unsigned)(height * 0.03));
	}

	void ControlKeysView::Init()
	{
		_linkText.SetText("Hold / to view controls.", 255, 255, 255, 0);
		for (int i = 0; i < 5; i++)
			_instructionsText.push_back(FontView("FONTS\\CALIBRI.TTF", 28));

		_instructionsText[0].SetText("Controls", 255, 255, 255, 0);
		_instructionsText[1].SetText("Up/Down/Left/Right - Common, figure it out yourself.", 255, 255, 255, 0);
		_instructionsText[2].SetText("N - Restart the game.", 255, 255, 255, 0);
		_instructionsText[3].SetText("V - Swap the views.", 255, 255, 255, 0);
		_instructionsText[4].SetText("NUM +/ENTER - Zoom In/Out",255, 255, 255, 0);
	}
}

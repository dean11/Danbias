/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_WIN32MOUSE_H
#define INPUT_WIN32MOUSE_H

#include "..\Mouse.h"
#include <Windows.h>

namespace Input
{
	class Win32Mouse :public Mouse
	{
	public:
		Win32Mouse();
		~Win32Mouse();

		bool IsBtnUp(Enum::SAMI key) override;
		bool IsBtnDown(Enum::SAMI key) override;
		int GetWheelDelta()  override;
		Struct::SAIPoint2D GetPixelPosition(Struct::SAIPoint2D targetMem = Struct::SAIPoint2D())  override;

		void ProccessMouseData (bool isDown, Enum::SAMI btn, int delta, Struct::SAIPoint2D velocity, unsigned int makeCode);

	private:
		struct Buttons
		{
			unsigned int makeCode;
			bool isDown;
		};
		static const int MAXBUTTONS = 25;
		Buttons buttons[25];
	};
}

#endif // !INPUT_WIN32MOUSE_H

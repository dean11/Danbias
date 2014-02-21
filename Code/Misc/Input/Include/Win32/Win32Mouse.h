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

		void ProccessMouseData (RAWMOUSE mouse);

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

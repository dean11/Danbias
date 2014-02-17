/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include "..\Keyboard.h"
#include <Windows.h>

namespace Input
{
	class Win32Keyboard :public Keyboard
	{
	public:
		Win32Keyboard();
		~Win32Keyboard();

		bool IsKeyUp (Enum::SAKI key) override;
		bool IsKeyDown (Enum::SAKI key) override;
		const wchar_t* GetAsText(Enum::SAKI key) override;

		void ProccessKeyboardData (bool isUp, Enum::SAKI key, unsigned int makeCode, bool isE0);

	private:
		struct Keys
		{
			bool isE0;
			bool isDown;
			unsigned int makecode;
		};
		static const int MAXKEYS = 256;
		Keys keys[MAXKEYS];
	};
}

#endif // !INPUT_KEYBOARD_H

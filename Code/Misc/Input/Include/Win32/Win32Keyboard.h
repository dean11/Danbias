/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include "..\Keyboard.h"
#define NOMINMAX
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
		wchar_t* GetAsText(Enum::SAKI key) override;

		void ProccessKeyboardData (RAWKEYBOARD keyboard);

	private:
		void MapKey(RAWKEYBOARD& rawKB, Enum::SAKI& out_key, bool& isE0);
		
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

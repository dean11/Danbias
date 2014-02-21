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
		Win32Keyboard(HWND target);
		~Win32Keyboard();

		bool IsKeyUp (Enum::SAKI key) override;
		bool IsKeyDown (Enum::SAKI key) override;
		wchar_t* GetAsText(Enum::SAKI key) override;
		void SetInputOptionType(Enum::InputOptionType options) override;
		bool IsInputOptionSuported(Enum::InputOptionType options) const override;

		void ProccessKeyboardData (RAWKEYBOARD keyboard);
		bool Create( );

	private:
		void MapKey(RAWKEYBOARD& rawKB, Enum::SAKI& out_key, bool& isE0);
		
		struct Keys
		{
			bool isE0;
			bool isDown;
			unsigned int makecode;
		};
		RAWINPUTDEVICE device;
		static const int MAXKEYS = 256;
		Keys keys[MAXKEYS];
	};
}

#endif // !INPUT_KEYBOARD_H

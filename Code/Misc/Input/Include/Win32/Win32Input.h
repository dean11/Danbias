/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_GUARD_RAW_INPUT_H
#define INCLUDE_GUARD_RAW_INPUT_H


#include "..\InputManager.h"
#include "Win32Keyboard.h"
#include "Win32Mouse.h"
#include <vector>
#define NOMINMAX
#include <Windows.h>
#include <map>

/**
*	TODO:
*		1. Cordinate system
*			1.1. Pixel cordinates DONE
*			1.2. 0 - 1 cordinate DONW
*			1.3. Origo in middle of the screen ( -1 to 1 )
*/

//RIDEV_INPUTSINK to receive all keyboard input regardless of focus

//dx = +2*(x/w) -1
//dx = -2*(y/h) +1


namespace Input
{
	enum RawInput_Usage
	{
		RawInput_Usage_pointer				= 1,
		RawInput_Usage_mouse				= 2,
		RawInput_Usage_joystick				= 4,
		RawInput_Usage_gamepad				= 5,
		RawInput_Usage_keyboard				= 6,
		RawInput_Usage_keypad				= 7,
		RawInput_Usage_multiAxisController	= 8,
		RawInput_Usage_TabletPCcontrols		= 9,
	};

	class Win32Input	:public InputManager
	{
	public:
		Win32Input();
		virtual~Win32Input();

		InputObject* CreateDevice(const Enum::SAIType inputType, Typedefs::WindowHandle targetApplication) override;
		void ToggleInputSystem(bool enable) override;
		void Destroy() override;

	private:
		std::vector<Win32Keyboard*> keyboard;
		std::vector<Win32Mouse*> mouse;

		bool enabled;
		HWND targetHwin;

	private:
		static Win32Input* instance;
		static LRESULT RawInputParser(HWND h, LPARAM l);
		static LRESULT CALLBACK RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l);
		static void WindowActivate(bool activate);
	};
}
#endif


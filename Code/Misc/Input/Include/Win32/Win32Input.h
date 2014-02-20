/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_GUARD_RAW_INPUT_H
#define INCLUDE_GUARD_RAW_INPUT_H


#include "..\InputManager.h"
#include "Win32Keyboard.h"
#include "Win32Mouse.h"
#include "Win32ApplicationKeyboard.h"
#include <vector>
#include <Windows.h>
#include <map>

/**
*	TODO:
*		1. Cordinate system
*			1.1. Pixel cordinates
*			1.2. 0 - 1 cordinate
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
	//	private:
	//		SubscribeList<INPUT_CALLBACK, const InputData*>*			_procInput;
	//
	//		bool														_enabled;
	//		bool														_mouseEnabled;
	//		bool														_KeyboardEnabled;
	//		bool														_exclusive;
	//		List<RawInputDeviceInstance>								_deviceList;
	//		
	//		List<InputData>											_mouseInput;
	//		List<InputData>											_keyboardInput;
	//		
	//		HHOOK														_msgHook;
	//
	//	private:
	//		Win32Input													();	
	//		~Win32Input													();
	//		
	//		bool _addDevice												(const RAWINPUTDEVICE* k, const int& count);
	//		RAWINPUT*_TranslateRawInput									(LPARAM l);
	//		void _proccessRawMouseData									(RAWMOUSE&);
	//		void _proccessRawKeyboardData								(RAWKEYBOARD&);
	//
	//		static LRESULT CALLBACK WM_INPUT_TRANSLATE					(int nCode, WPARAM wParam, LPARAM lParam);
	//
	//	public:
	//
	//		static Win32Input* Self										();
	//		static void Destroy											();
	//		
	//		const wchar_t*			Input_GetError						() const;
	//
	//		bool					Input_AddDevice						(IN const HWND& targetApplication);
	//		bool					Input_AddDevice						(IN const RAWINPUTDEVICE*, IN const int&);
	//
	//		void					Input_Subscribe						(IN INPUT_CALLBACK fnc);
	//		void					Input_Unsubscribe					(IN INPUT_CALLBACK fnc);
	//
	//		void					Input_Disable						();
	//		void					Input_Enable						();
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
		bool exclusive;
		HWND targetHwin;

	private:
		static Win32Input* instance;
		static void RawInputParser(HWND h, LPARAM l);
		static LRESULT CALLBACK RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l);
		static void WindowActivate(bool activate);
	};
}
#endif


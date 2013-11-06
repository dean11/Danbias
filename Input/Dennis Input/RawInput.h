#ifndef INCLUDE_GUARD_RAW_INPUT_H
#define INCLUDE_GUARD_RAW_INPUT_H

#include "RawInputWrapper.h"
#include "misc.h"
#include <vector>

/**
*	TODO:
*		1. Origo in middle of the screen (-1 to 1)
*/


//dx = +2*(x/w) -1
//dx = -2*(y/h) +1

template<class FNC, class PARAM>
struct SubscribeList
{
	FNC					fnc;
	PARAM				param;
	SubscribeList*		next;
};
struct RawInputDeviceInstance
{
	RAWINPUTDEVICE description;
};


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

class RawInput	:public RawInputWrapper
{
	private:
		SubscribeList<INPUT_CALLBACK, const RawInputData*>*			_procInput;

		bool														_enabled;
		bool														_mouseEnabled;
		bool														_KeyboardEnabled;
		bool														_exclusive;
		const wchar_t*												_errorMsg;
		List<RawInputDeviceInstance>								_deviceList;

		List<RawInputData>											_mouseInput;
		List<RawInputData>											_keyboardInput;
		
		HHOOK														_msgHook;

	private:
		RawInput													();	
		~RawInput													();
		
		bool _addDevice												(const RAWINPUTDEVICE* k, const int& count);
		RAWINPUT*_TranslateRawInput									(LPARAM l);
		void _proccessRawMouseData									(RAWMOUSE&);
		void _proccessRawKeyboardData								(RAWKEYBOARD&);

		static LRESULT CALLBACK WM_INPUT_TRANSLATE					(int nCode, WPARAM wParam, LPARAM lParam);

	public:

		static RawInput* Self										();
		static void Destroy											();
		
		const wchar_t*			Input_GetError						() const;

		bool					Input_AddDevice						(IN const HWND& targetApplication);
		bool					Input_AddDevice						(IN const RAWINPUTDEVICE*, IN const int&);

		void					Input_Subscribe						(IN INPUT_CALLBACK fnc);
		void					Input_Unsubscribe					(IN INPUT_CALLBACK fnc);

		void					Input_Disable						();
		void					Input_Enable						();

		void					Input_Read							();

		void					Mouse_Show							();
		void					Mouse_Hide							();
		void					Mouse_Lock							();
		void					Mouse_Unlock						();
		void					Mouse_IsBtnPressed					(IN RIK);
		int						Mouse_WheelDelta					();
		POINT					Mouse_Position						();
		void					Mouse_Enable						();
		void					Mouse_Disable						();

		bool					Keyboard_KeyUp						(IN RIK key);
		bool					Keyboard_KeyDown					(IN RIK key);
		void					Keyboard_Enable						();
		void					Keyboard_Disable					();
};
#endif


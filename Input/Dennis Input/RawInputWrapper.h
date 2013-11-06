#ifndef RAW_INPUT_H
#define RAW_INPUT_H

#include <Windows.h>



/*********************************************************************/
/***************************** Keycodes ******************************/
/*********************************************************************/
//! Contains keycodes
enum RIK
{
	RIK_Backspace		= 0x08,
	RIK_Tab				= 0x09,
	RIK_Enter			= 0x0D,
	RIK_Shift			= 0x10,
	RIK_Ctrl			= 0x11,
	RIK_Alt				= 0x12,
	RIK_Pause			= 0x13,
	RIK_CapsLock		= 0x14,
	RIK_Escape			= 0x1B,
	RIK_Space			= 0x20,
	RIK_PgUp			= 0x21,
	RIK_PgDown			= 0x22,
	RIK_End				= 0x23,
	RIK_Home			= 0x24,
	RIK_Left			= 0x25,
	RIK_Up				= 0x26,
	RIK_Right			= 0x27,
	RIK_Down			= 0x28,
	RIK_PrintScreen		= 0x2C,
	RIK_Insert			= 0x2D,
	RIK_Delete			= 0x2E,
	RIK_0				= 0x30,
	RIK_1				= 0x31,
	RIK_2				= 0x32,
	RIK_3				= 0x33,
	RIK_4				= 0x34,
	RIK_5				= 0x35,
	RIK_6				= 0x36,
	RIK_7				= 0x37,
	RIK_8				= 0x38,
	RIK_9				= 0x39,
	RIK_A				= 0x41,
	RIK_B				= 0x42,
	RIK_C				= 0x43,
	RIK_D				= 0x44,
	RIK_E				= 0x45,
	RIK_F				= 0x46,
	RIK_G				= 0x47,
	RIK_H				= 0x48,
	RIK_I				= 0x49,
	RIK_J				= 0x4A,
	RIK_K				= 0x4B,
	RIK_L				= 0x4C,
	RIK_M				= 0x4D,
	RIK_N				= 0x4E,
	RIK_O				= 0x4F,
	RIK_P				= 0x50,
	RIK_Q				= 0x51,
	RIK_R				= 0x52,
	RIK_S				= 0x53,
	RIK_T				= 0x54,
	RIK_U				= 0x55,
	RIK_V				= 0x56,
	RIK_W				= 0x57,
	RIK_X				= 0x58,
	RIK_Y				= 0x59,
	RIK_Z				= 0x5A,
	RIK_LeftWindows		= 0x5B,
	RIK_RightWindows	= 0x5C,
	RIK_Numpad0			= 0x60,
	RIK_Numpad1			= 0x61,
	RIK_Numpad2			= 0x62,
	RIK_Numpad3			= 0x63,
	RIK_Numpad4			= 0x64,
	RIK_Numpad5			= 0x65,
	RIK_Numpad6			= 0x66,
	RIK_Numpad7			= 0x67,
	RIK_Numpad8			= 0x68,
	RIK_Numpad9			= 0x69,
	RIK_Multiply		= 0x6A,
	RIK_Plus			= 0x6B,
	RIK_Subtract		= 0x6D,
	RIK_Decimal			= 0x6E,
	RIK_Divide			= 0x6F,
	RIK_F1				= 0x70,
	RIK_F2				= 0x71,
	RIK_F3				= 0x72,
	RIK_F4				= 0x73,
	RIK_F5				= 0x74,
	RIK_F6				= 0x75,
	RIK_F7				= 0x76,
	RIK_F8				= 0x77,
	RIK_F9				= 0x78,
	RIK_F10				= 0x79,
	RIK_F11				= 0x7A,
	RIK_F12				= 0x7B,
	RIK_F13				= 0x7C,
	RIK_F14				= 0x7D,
	RIK_F15				= 0x7E,
	RIK_F16				= 0x7F,
	RIK_F17				= 0x80,
	RIK_F18				= 0x81,
	RIK_F19				= 0x82,
	RIK_F20				= 0x83,
	RIK_F21				= 0x84,
	RIK_F22				= 0x85,
	RIK_F23				= 0x86,
	RIK_F24				= 0x87,
	RIK_Numlock			= 0x90,
	RIK_ScrlLock		= 0x91,
	RIK_LeftShift		= 0xA0,
	RIK_RightShift		= 0xA1,
	RIK_LeftCtrl		= 0xA2,
	RIK_RightCtrl		= 0xA3,

	COUNT
};
enum RIM
{
	RIM_LeftBtn			= 0x02,
	RIM_MiddleBtn		= 0x10,
	RIM_RightBtn		= 0x04,
	RIM_Scroll			= 0x0400,
};
/*********************************************************************/


/**************************************************************************/
/***************************** Callback data ******************************/
/**************************************************************************/
enum InputType
{
	InputType_Keyboard,
	InputType_Mouse,
	InputType_HID,
};
struct RawMouseData
{
	bool shift;
	bool alt;
	bool ctrl;

	RIM key;
	bool pressed;

	int MousePos_X;
	int MousePos_Y;

	int delta;
};
struct RawKeyData
{
	bool shift;
	bool alt;
	bool ctrl;

	RIK key;
	bool released;
};
struct RawInputData
{
	InputType type;
	union
	{
		RawMouseData	MOUSE_DATA;
		RawKeyData		KEYBOARD_DATA;
		RAWHID			HID_DATA;
	} data;
};
typedef void(*INPUT_CALLBACK)(const RawInputData*);
/*********************************************************************/

class RawInputWrapper
{
	public:
		static RawInputWrapper*			Self							();
		static void						Destroy							();
	
		virtual const wchar_t*			Input_GetError					() const													PURE;

		virtual bool					Input_AddDevice					(IN const HWND& targetApplication)							PURE;
		virtual bool					Input_AddDevice					(IN const RAWINPUTDEVICE* d, IN const int& count)			PURE;

		virtual void					Input_Subscribe					(IN INPUT_CALLBACK fnc)										PURE;
		virtual void					Input_Unsubscribe				(IN INPUT_CALLBACK fnc)										PURE;

		virtual void					Input_Disable					()															PURE;
		virtual void					Input_Enable					()															PURE;

		virtual void					Input_Read						()															PURE;

		virtual void					Mouse_Show						()															PURE;
		virtual void					Mouse_Hide						()															PURE;
		virtual void					Mouse_Lock						()															PURE;
		virtual void					Mouse_Unlock					()															PURE;
		virtual void					Mouse_IsBtnPressed				(IN RIK key)												PURE;
		virtual int						Mouse_WheelDelta				()															PURE;
		virtual POINT					Mouse_Position					()															PURE;
		virtual void					Mouse_Enable					()															PURE;
		virtual void					Mouse_Disable					()															PURE;
																																	
		virtual bool					Keyboard_KeyUp					(IN RIK key)												PURE;
		virtual bool					Keyboard_KeyDown				(IN RIK key)												PURE;
		virtual void					Keyboard_Enable					()															PURE;
		virtual void					Keyboard_Disable				()															PURE;

		
};


#endif


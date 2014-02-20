/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Input.h"
#include "..\..\Include\Keyboard.h"
#include <WindowsX.h>
#include <Windows.h>
#include <algorithm>
#include <ctime>

#include <fstream>
#include <iostream>

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;

/*
bool Win32Input::Input_AddDevice(IN const HWND& targetApplication)
{
	assert(targetApplication != 0);
	static const UINT c = 2;
	RAWINPUTDEVICE devices[c] = 
	{ 
		{ 0x01, RawInput_Usage_keyboard, RIDEV_NOLEGACY, targetApplication },
		{ 0x01, RawInput_Usage_mouse, RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE, targetApplication }
	};

	if(! _addDevice( devices , c ) )	return false;

	ShowCursor(FALSE);
	//RECT r;
	//GetWindow
	//GetWindowRect(
	 
	return true;
}
bool Win32Input::Input_AddDevice(IN const RAWINPUTDEVICE* d, const int& count)
{
	for (int i = 0; i < count; i++)
		if(!d[i].hwndTarget)
		{
			this->_errorMsg = L"Must specify target application";
			return false;
		}
	if(! _addDevice( d, count ) )	return false;

	return true;
}

//Win32InputDEVICE d = { 0x01, type, RIDEV_REMOVE, NULL };
//this->_errorMsg = L"Failed to unregister device";


void Win32Input::Input_Disable()	   
{
	this->_enabled = false;
}
void Win32Input::Input_Enable()	   
{
	this->_enabled = true;
}

void Win32Input::Input_Read()
{
	//for (int i = 0; i < this->_idleKeyData.size(); i++)
	//	this->_proccessRawKeyboardData(this->_idleKeyData.pop());
	//for (int i = 0; i < this->_idleMouseData.size(); i++)
	//	this->_proccessRawMouseData(this->_idleMouseData.pop());
	//
	//this->_idleKeyData.clear();
	//this->_idleMouseData.clear();
}


bool Win32Input::_addDevice						(const RAWINPUTDEVICE* k, const int& count)
{
	if(RegisterRawInputDevices(k, count, sizeof(RAWINPUTDEVICE)) == FALSE)
	{
		DWORD h = GetLastError();
		INPUT_EXCEPT( L"Failed to register device" );
		return false;
	}

	for (int q = 0; q < count; q++)
	{
		RawInputDeviceInstance i;
		memcpy(&i.description, &k[q], sizeof(RAWINPUTDEVICE));
		this->_deviceList.push(i);
	}

	return true;
}
*/

Win32Input *Win32Input::instance = 0;
void MapKey(RAWKEYBOARD& rawKB, bool& out_isUp, SAKI& out_key, unsigned int& sCode, bool& isE0)
{
	//------------------------------------------------------------------------------------//
	// http://molecularmusings.wordpress.com/2011/09/05/properly-handling-keyboard-input/ //
	//------------------------------------------------------------------------------------//

	UINT virtualKey = rawKB.VKey;
	UINT scanCode = rawKB.MakeCode;
	UINT flags = rawKB.Flags;

	if (virtualKey == 255)
	{
		// discard "fake keys" which are part of an escaped sequence
		return;
	}
	else if (virtualKey == VK_SHIFT)
	{
		// correct left-hand / right-hand SHIFT
		virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
	}
	else if (virtualKey == VK_NUMLOCK)
	{
		// correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
		scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
	}

	// e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
	// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
	isE0 = ((flags & RI_KEY_E0) != 0);
	const bool isE1 = ((flags & RI_KEY_E1) != 0);
 
	if (isE1)
	{
		// for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
		// however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
		if (virtualKey == VK_PAUSE) scanCode = 0x45;
		else						scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
	}

	switch (virtualKey)
	{
		// right-hand CONTROL and ALT have their e0 bit set
		case VK_CONTROL:
			if (isE0)	out_key = SAKI_RightCtrl;
			else		out_key = SAKI_LeftCtrl;
		break;
 
		case VK_MENU:
			if (isE0)	out_key = SAKI_RightAlt;
			else		out_key = SAKI_LeftAlt;
		break;
 
		// NUMPAD ENTER has its e0 bit set
		case VK_RETURN:
			if (isE0)	out_key = SAKI_NumpadEnter;
		break;
 
		// the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
		// corresponding keys on the NUMPAD will not.
		case VK_INSERT:
			if (!isE0)	out_key = SAKI_Numpad0;
		break;
 
		case VK_DELETE:
			if (!isE0)	out_key = SAKI_NumpadDecimal;
		break;
 
		case VK_HOME:
			if (!isE0)	out_key = SAKI_Numpad7;
		break;
 
		case VK_END:
			if (!isE0)	out_key = SAKI_Numpad1;
		break;
 
		case VK_PRIOR:
			if (!isE0)	out_key = SAKI_Numpad9;
		break;
 
		case VK_NEXT:
			if (!isE0)	out_key = SAKI_Numpad3;
		break;
 
		// the standard arrow keys will always have their e0 bit set, but the
		// corresponding keys on the NUMPAD will not.
		case VK_LEFT:
			if (!isE0)	out_key = SAKI_Numpad4;
		break;
 
		case VK_RIGHT:
			if (!isE0)	out_key = SAKI_Numpad6;
		break;
 
		case VK_UP:
			if (!isE0)	out_key = SAKI_Numpad8;
		break;
 
		case VK_DOWN:
			if (!isE0)	out_key = SAKI_Numpad2;
		break;
 
		// NUMPAD 5 doesn't have its e0 bit set
		case VK_CLEAR:
			if (!isE0)	out_key = SAKI_Numpad5;
		break;
		case 0x03	:	//VK_CANCEL				
		break;
		case 0x08	:	//VK_BACK	
			out_key = SAKI_Backspace;
		break;	
		case 0x09	:	//VK_TAB	
			out_key = SAKI_Tab;
		break;	
		case 0x10	:	//VK_SHIFT	
			out_key = SAKI_LeftShift;
			out_key = SAKI_RightShift;
		break;	
		case 0x13	:	//VK_PAUSE	
			out_key = SAKI_Pause;
		break;
		case 0x14	:	//VK_CAPITAL
			out_key = SAKI_CapsLock;
		break;	
		case 0x15	:	//VK_KANA				
		break;	
		case 0x1B	:	//VK_ESCAPE	
			out_key = SAKI_Escape;
		break;
		case 0x1C	:	//VK_CONVERT			
		break;	
		case 0x1D	:	//VK_NONCONVERT			
		break;
		case 0x1E	:	//VK_ACCEPT				
		break;
		case 0x1F	:	//VK_MODECHANGE			
		break;
		case 0x20	:	//VK_SPACE	
			out_key = SAKI_Space;
		break;	
		case 0x29	:	//VK_SELECT				
		break;
		case 0x2A	:	//VK_PRINT	
			out_key = SAKI_PrintScreen;
		break;
		case 0x2B	:	//VK_EXECUTE			
		break;	
		case 0x2C	:	//VK_SNAPSHOT			
		break;	
		case 0x2F	:	//VK_HELP				
		break;	
		case 0x30	:	//0 key	
			out_key = SAKI_0;
		break;
		case 0x31	:	//1 key	
			out_key = SAKI_1;
		break;
		case 0x32	:	//2 key	
			out_key = SAKI_2;
		break;
		case 0x33	:	//3 key	
			out_key = SAKI_3;
		break;
		case 0x34	:	//4 key	
			out_key = SAKI_4;
		break;
		case 0x35	:	//5 key	
			out_key = SAKI_5;
		break;
		case 0x36	:	//6 key		
			out_key = SAKI_6;
		break;
		case 0x37	:	//7 key	
			out_key = SAKI_7;
		break;
		case 0x38	:	//8 key		
			out_key = SAKI_8;
		break;
		case 0x39	:	//9 key		
			out_key = SAKI_9;
		break;
		case 0x41	:	//A key		
			out_key = SAKI_A;
		break;
		case 0x42	:	//B key	
			out_key = SAKI_B;
		break;
		case 0x43	:	//C key		
			out_key = SAKI_C;
		break;
		case 0x44	:	//D key	
			out_key = SAKI_D;
		break;
		case 0x45	:	//E key	
			out_key = SAKI_E;
		break;
		case 0x46	:	//F key		
			out_key = SAKI_F;
		break;
		case 0x47	:	//G key		
			out_key = SAKI_G;
		break;
		case 0x48	:	//H key	
			out_key = SAKI_H;
		break;
		case 0x49	:	//I key		
			out_key = SAKI_I;
		break;
		case 0x4A	:	//J key	
			out_key = SAKI_J;
		break;
		case 0x4B	:	//K key	
			out_key = SAKI_K;
		break;
		case 0x4C	:	//L key	
			out_key = SAKI_L;
		break;
		case 0x4D	:	//M key	
			out_key = SAKI_M;
		break;
		case 0x4E	:	//N key	
			out_key = SAKI_N;
		break;
		case 0x4F	:	//O key	
			out_key = SAKI_O;
		break;
		case 0x50	:	//P key	
			out_key = SAKI_P;				
		break;
		case 0x51	:	//Q key	
			out_key = SAKI_Q;				
		break;
		case 0x52	:	//R key	
			out_key = SAKI_R;				
		break;
		case 0x53	:	//S key	
			out_key = SAKI_S;				
		break;
		case 0x54	:	//T key	
			out_key = SAKI_T;				
		break;
		case 0x55	:	//U key	
			out_key = SAKI_U;				
		break;
		case 0x56	:	//V key	
			out_key = SAKI_V;				
		break;
		case 0x57	:	//W key	
			out_key = SAKI_W;				
		break;
		case 0x58	:	//X key	
			out_key = SAKI_X;				
		break;
		case 0x59	:	//Y key	
			out_key = SAKI_Y;				
		break;
		case 0x5A	:	//Z key	
			out_key = SAKI_Z;				
		break;
		case 0x5B	:	//VK_LWIN				
		break;	
		case 0x5C	:	//VK_RWIN				
		break;	
		case 0x5D	:	//VK_APPS				
		break;	
		case 0x5F	:	//VK_SLEEP				
		break;
		case 0x60	:	//VK_NUMPAD0
			out_key = SAKI_Numpad0;
		break;	
		case 0x61	:	//VK_NUMPAD1	
			out_key = SAKI_Numpad1;
		break;	
		case 0x62	:	//VK_NUMPAD2	
			out_key = SAKI_Numpad2;
		break;	
		case 0x63	:	//VK_NUMPAD3	
			out_key = SAKI_Numpad3;
		break;	
		case 0x64	:	//VK_NUMPAD4	
			out_key = SAKI_Numpad4;
		break;	
		case 0x65	:	//VK_NUMPAD5	
			out_key = SAKI_Numpad5;
		break;	
		case 0x66	:	//VK_NUMPAD6	
			out_key = SAKI_Numpad6;
		break;	
		case 0x67	:	//VK_NUMPAD7	
			out_key = SAKI_Numpad7;
		break;	
		case 0x68	:	//VK_NUMPAD8	
			out_key = SAKI_Numpad8;
		break;	
		case 0x69	:	//VK_NUMPAD9	
			out_key = SAKI_Numpad9;
		break;	
		case 0x6A	:	//VK_MULTIPLY	
			out_key = SAKI_NumpadMultiply;		
		break;	
		case 0x6B	:	//VK_ADD	
			out_key = SAKI_NumpadPlus;			
		break;	
		case 0x6C	:	//VK_SEPARATOR			
		break;
		case 0x6D	:	//VK_SUBTRACT
			out_key = SAKI_NumpadSubtract;			
		break;	
		case 0x6E	:	//VK_DECIMAL	
			out_key = SAKI_NumpadDecimal;		
		break;	
		case 0x6F	:	//VK_DIVIDE	
			out_key = SAKI_NumpadDivide;			
		break;
		case 0x70	:	//VK_F1	
			out_key = SAKI_F1;				
		break;
		case 0x71	:	//VK_F2	
			out_key = SAKI_F2;				
		break;
		case 0x72	:	//VK_F3	
			out_key = SAKI_F3;				
		break;
		case 0x73	:	//VK_F4	
			out_key = SAKI_F4;				
		break;
		case 0x74	:	//VK_F5	
			out_key = SAKI_F5;				
		break;
		case 0x75	:	//VK_F6	
			out_key = SAKI_F6;				
		break;
		case 0x76	:	//VK_F7	
			out_key = SAKI_F7;				
		break;
		case 0x77	:	//VK_F8	
			out_key = SAKI_F8;				
		break;
		case 0x78	:	//VK_F9	
			out_key = SAKI_F9;				
		break;
		case 0x79	:	//VK_F10
			out_key = SAKI_F10;				
		break;	
		case 0x7A	:	//VK_F11
			out_key = SAKI_F11;				
		break;	
		case 0x7B	:	//VK_F12
			out_key = SAKI_F12;				
		break;	
		case 0x7C	:	//VK_F13
			out_key = SAKI_F13;				
		break;	
		case 0x7D	:	//VK_F14
			out_key = SAKI_F14;				
		break;	
		case 0x7E	:	//VK_F15
			out_key = SAKI_F15;				
		break;	
		case 0x7F	:	//VK_F16
			out_key = SAKI_F16;				
		break;	
		case 0x80	:	//VK_F17	
			out_key = SAKI_F17;			
		break;	
		case 0x81	:	//VK_F18
			out_key = SAKI_F18;				
		break;	
		case 0x82	:	//VK_F19
			out_key = SAKI_F19;				
		break;	
		case 0x83	:	//VK_F20	
			out_key = SAKI_F20;			
		break;	
		case 0x84	:	//VK_F21	
			out_key = SAKI_F21;			
		break;	
		case 0x85	:	//VK_F22	
			out_key = SAKI_F22;			
		break;	
		case 0x86	:	//VK_F23	
			out_key = SAKI_F23;			
		break;	
		case 0x87	:	//VK_F24	
			out_key = SAKI_F24;			
		break;	
		case 0x90	:	//VK_NUMLOCK
			out_key = SAKI_Numlock;			
		break;	
		case 0x91	:	//VK_SCROLL
			out_key = SAKI_ScrlLock;				
		break;
		case 0xA0	:	//VK_LSHIFT	
			out_key = SAKI_LeftShift;			
		break;
		case 0xA1	:	//VK_RSHIFT		
			out_key = SAKI_RightShift;		
		break;
		case 0xA2	:	//VK_LCONTROL	
			out_key = SAKI_LeftCtrl;
		break;	
		case 0xA3	:	//VK_RCONTROL	
			out_key = SAKI_RightCtrl;
		break;	
		case 0xA4	:	//VK_LMENU	
			out_key = SAKI_LeftAlt;
		break;
		case 0xA5	:	//VK_RMENU
			out_key = SAKI_RightCtrl;
		break;
		case 0xAD	:	//VK_VOLUME_MUTE	
			out_key = SAKI_VolumeMute;
		break;	
		case 0xAE	:	//VK_VOLUME_DOWN	
			out_key = SAKI_VolumeDown;
		break;	
		case 0xAF	:	//VK_VOLUME_UP	
			out_key = SAKI_VolumeUp;
		break;
		case 0xB0	:	//VK_MEDIA_NEXT_TRACK	
			out_key = SAKI_MediaNext;
		break;	
		case 0xB1	:	//VK_MEDIA_PREV_TRACK	
			out_key = SAKI_MediaPrev;
		break;	
		case 0xB2	:	//VK_MEDIA_STOP		
			out_key = SAKI_MediaStop;
		break;	
		case 0xB3	:	//VK_MEDIA_PLAY_PAUSE	
			out_key = SAKI_MediaPlayPause;
		break;

		case 0xBB://VK_OEM_PLUS		
		break;
		case 0xBC://VK_OEM_COMMA	
		break;
		case 0xBD://VK_OEM_MINUS	
		break;
		case 0xBE://VK_OEM_PERIOD	
		break;
		case 0xBA://VK_OEM_1		
		break;
		case 0xBF://VK_OEM_2	
		break;	
		case 0xC0://VK_OEM_3	
		break;	
		case 0xDB://VK_OEM_4	
		break;	
		case 0xDC://VK_OEM_5	
		break;	
		case 0xDD://VK_OEM_6
		break;	
		case 0xDE://VK_OEM_7	
		break;	
		case 0xDF://VK_OEM_8	
		break;
	}

	out_isUp = ((flags & RI_KEY_BREAK) != 0);
	rawKB.MakeCode = scanCode;
	sCode = scanCode;
}
void MapButton(RAWMOUSE& rawMouse, bool &isUp, Enum::SAMI& btn, int& delta, Struct::SAIPointInt2D& vel, unsigned int& mcode)
{
	if(rawMouse.lLastX != 0 || rawMouse.lLastY != 0)
	{
		vel.x = rawMouse.lLastX;
		vel.y = rawMouse.lLastY;
	}
	if( rawMouse.usButtonFlags > 0 )
	{
//--------------------------------------------------------------------------------------
		//Mouse button pressed
		if(rawMouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)	
		{
			btn = SAMI_MouseLeftBtn;
			isUp = false;
		}
		else if(rawMouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_DOWN)	
		{	
			btn = SAMI_MouseMiddleBtn;
			isUp = false;
		}
		else if(rawMouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)	
		{	
			btn = SAMI_MouseRightBtn;
			isUp = false;
		}
//--------------------------------------------------------------------------------------
		//Mouse button Released
		else if(rawMouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)	
		{
			btn = SAMI_MouseLeftBtn;
			isUp = true;
		}
		else if(rawMouse.usButtonFlags == RI_MOUSE_MIDDLE_BUTTON_UP)	
		{
			btn = SAMI_MouseMiddleBtn;
			isUp = true;
		}
		else if(rawMouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)	
		{
			btn = SAMI_MouseRightBtn;
			isUp = true;
		}
//--------------------------------------------------------------------------------------
		else if (rawMouse.usButtonFlags == RI_MOUSE_WHEEL)
		{
			delta = ((int)rawMouse.usButtonData);
	
			if(delta > 120)	delta = -1;
			else			delta = 1;
		}
	}
}
void Win32Input::RawInputParser(HWND h, LPARAM l)
{
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{
		return;
	} 

	//Create and read the raw input data
	LPBYTE rawBufferIn = new BYTE[bufferSize];
	UINT readBytes = GetRawInputData((HRAWINPUT)l, RID_INPUT, rawBufferIn, &bufferSize,  sizeof(RAWINPUTHEADER));
	if ( readBytes != bufferSize )
	{
		delete [] rawBufferIn;
		return;
	}

	RAWINPUT* raw = (RAWINPUT*)rawBufferIn;
	 

	if(!Win32Input::instance->enabled)
	{
		if(FAILED ( DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER)) ) )	
		{
			
		}
	}
	else
	{
		if(raw->header.dwType == RIM_TYPEMOUSE)	
		{
			for (unsigned int i = 0; i < Win32Input::instance->mouse.size(); i++)
			{
				bool isUp = true;
				Enum::SAMI btn = Enum::SAMI_Unknown;
				int delta = 0;
				Struct::SAIPointInt2D vel;
				unsigned int mcode = 0;
				MapButton(raw->data.mouse, isUp, btn, delta, vel, mcode);

				Win32Input::instance->mouse[i]->ProccessMouseData(isUp, btn, delta, vel, mcode);
			}
		}
		else if(raw->header.dwType == RIM_TYPEKEYBOARD)	
		{	
			for (unsigned int i = 0; i < Win32Input::instance->keyboard.size(); i++)
			{
				bool isUp;
				SAKI key = SAKI_Unknown;
				unsigned int makeCode;
				bool isE0;
				MapKey(raw->data.keyboard, isUp, key, makeCode, isE0);
				Win32Input::instance->keyboard[i]->ProccessKeyboardData(isUp, key, makeCode, isE0);
			}
		}
	}

	delete raw;
}
LRESULT CALLBACK Win32Input::RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
	switch (m)
	{
		case WM_INPUT:
			Win32Input::instance->RawInputParser(h, l);
		break;
		case WM_ACTIVATE:
			Win32Input::instance->WindowActivate((w == TRUE));
		break;
		case WM_CREATE:
			Win32Input::instance->WindowActivate(true);
		break;
	}

	return DefWindowProc(h, m, w, l);
}
void Win32Input::WindowActivate(bool activate)
{
	if(activate)
	{
		ShowCursor(0);
	}
	else
	{
		ShowCursor(0);
	}
}


Win32Input::Win32Input()
{
	if(!this->instance) 
	{
		this->instance = this;
	}

	WNDCLASSEXW wc;
	wc.cbSize		 = sizeof(WNDCLASSEXW);
	wc.hIconSm		 = NULL;
	wc.style         = NULL;
	wc.lpfnWndProc   = RawWindowCallback; 
	wc.cbClsExtra    = NULL;
	wc.cbWndExtra    = NULL;
	wc.hInstance     = (HINSTANCE)GetModuleHandle(0);
	wc.hIcon         = NULL;
	wc.hCursor       = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = L"RawInputCallbackFunc";

	if( !RegisterClassExW(&wc) )
	{ /*wrong*/ }
}
Win32Input::~Win32Input()
{}
InputObject* Win32Input::CreateDevice(const SAIType inputType, Typedefs::WindowHandle targetApplication)
{
	if(!this->instance->targetHwin)
	{
		this->targetHwin = CreateWindowExW(  0, L"RawInputCallbackFunc" ,  NULL, NULL, NULL, NULL, NULL, 
												NULL, (HWND)targetApplication, NULL, (HINSTANCE)GetModuleHandle(0), NULL ); 
	}
	InputObject* val = 0;
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.hwndTarget = this->instance->targetHwin;

	switch (inputType)
	{
		case SAIType_Keyboard:
		{
			rid.usUsage = RawInput_Usage_keyboard;
			rid.dwFlags = RIDEV_NOLEGACY;
			if(RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == TRUE)
			{
				Win32Keyboard* obj = new Win32Keyboard();
				this->keyboard.push_back(obj);
				val = obj;
			}
			else
			{
				return 0;
			}
		}
		break;

		case SAIType_Mouse:
		{
			Win32Mouse* obj = new Win32Mouse();
			if(!obj->Create(this->targetHwin))
			{
				delete obj;
				return 0;
			}

			this->mouse.push_back(obj);
			val = obj;
		}
		break;
	}

	return val;
}
void Win32Input::ToggleInputSystem(bool enable)
{
	this->enabled = enable;
}
void Win32Input::Destroy ()
{
	ShowCursor(true);
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);
	ClipCursor(&r);

	for (unsigned int i = 0; i < this->keyboard.size(); i++)
	{
		delete this->keyboard[i];
	}
	for (unsigned int i = 0; i < this->mouse.size(); i++)
	{
		delete this->mouse[i];
	}
	this->mouse.resize(0);
	this->keyboard.resize(0);
}


/* This method is used with hooks!

LRESULT CALLBACK RawInput::WM_INPUT_TRANSLATE	(int nCode, WPARAM wParam, LPARAM lparam)
{
	if (nCode < 0)		return CallNextHookEx(RawInput::Self()->_msgHook, nCode, wParam, lparam); 


	MSG *m = (MSG*)lparam;
	
	if(m->message == WM_INPUT)
	{
		RAWINPUT* raw = RawInput::Self()->_TranslateRawInput(m->lParam);

		if(!raw) goto nextHook;
		if(!RawInput::Self()->Self()->_enabled)
		{
			if(FAILED ( DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER)) ) )	
				RawInput::Self()->_errorMsg = L"Failed to proccess default raw input";
			goto _final;
		}
		//	 if(raw->header.dwType == RIM_TYPEMOUSE)	RawInput::Self()->_idleMouseData.insert(raw->data.mouse);
		//else if(raw->header.dwType == RIM_TYPEKEYBOARD)	RawInput::Self()->_proccessRawKeyboardData(raw->data.keyboard);
		

		_final:

		//if(FAILED ( DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER)) ) )
		//		RawInput::Self()->_errorMsg = L"Failed to proccess default raw input";

		delete raw;
	}
	else if (m->message == WM_QUIT)
	{
		if(UnhookWindowsHookEx(RawInput::Self()->_msgHook) == FALSE)
		{
			RawInput::Self()->_errorMsg = L"Failed to unhook message hook!";
		}
	}


	nextHook:
	return CallNextHookEx(RawInput::Self()->_msgHook, nCode, wParam, lparam); 
}
*/





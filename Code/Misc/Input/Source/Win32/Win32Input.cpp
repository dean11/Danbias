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


Win32Input *Win32Input::instance = 0;

LRESULT Win32Input::RawInputParser(HWND h, LPARAM l)
{
	LRESULT val = 0;
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{ return 0; } 

	//Create and read the raw input data
	LPBYTE rawBufferIn = new BYTE[bufferSize];
	UINT readBytes = GetRawInputData((HRAWINPUT)l, RID_INPUT, rawBufferIn, &bufferSize,  sizeof(RAWINPUTHEADER));
	if ( readBytes != bufferSize )
	{
		delete [] rawBufferIn;
		return 0;
	}

	RAWINPUT* raw = (RAWINPUT*)rawBufferIn;

	if(!Win32Input::instance->enabled)
	{
		val = DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));	
	}
	else
	{
		if(raw->header.dwType == RIM_TYPEMOUSE)	
		{
			bool once = false;
			for (unsigned int i = 0; i < Win32Input::instance->mouse.size(); i++)
			{
				if(Win32Input::instance->mouse[i]->IsActive())
				{
					Win32Input::instance->mouse[i]->ProccessMouseData(raw->data.mouse);
				}
				else
				{
					if(!once) val = DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
				}
			}
		}
		else if(raw->header.dwType == RIM_TYPEKEYBOARD)	
		{	
			bool once = false;
			for (unsigned int i = 0; i < Win32Input::instance->keyboard.size(); i++)
			{
				if(Win32Input::instance->keyboard[i]->IsActive())
				{
					Win32Input::instance->keyboard[i]->ProccessKeyboardData(raw->data.keyboard);
				}
				else
				{
					if(!once) val = DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));
				}
			}
		}
	}

	delete raw;
	return val;
}
LRESULT CALLBACK Win32Input::RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
	LRESULT val = 0;
	switch (m)
	{
		case WM_INPUT:
			return Win32Input::instance->RawInputParser(h, l);
		break;

		case WM_KEYDOWN:
			val = 0;
		break;

		case WM_CHAR:
			val = 0;
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
	this->targetHwin = 0;
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
			rid.usUsage = RawInput_Usage_mouse;
			rid.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;
			if(RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == TRUE)
			{
				int i = 0;
				val = (InputObject*)1;
				Win32Mouse* obj = new Win32Mouse();
				this->mouse.push_back(obj);
				val = obj;
			}
			else
			{
				return 0;
			}
		}
		break;

		//case SAIType_ApplicationKeyboard:
		//	//val = new Win32ApplicationKeyboard();
		//	break;
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


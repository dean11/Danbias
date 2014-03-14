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
TRACKMOUSEEVENT tme;

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
	UINT te = 0;
	if(m != WM_INPUT)
		te = m;
	
	switch (m)
	{
		case WM_INPUT:
			return Win32Input::instance->RawInputParser(h, l);
		break;
		
		case WM_NCACTIVATE:
		case WM_ACTIVATEAPP:
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
		//SetCursor(0);
		for (unsigned int i = 0; i < Win32Input::instance->mouse.size(); i++)
		{
			Win32Input::instance->mouse[i]->ToggleDefault(false);
		}
		for (unsigned int i = 0; i < Win32Input::instance->keyboard.size(); i++)
		{
			Win32Input::instance->keyboard[i]->ResetKeys();
		}
		
	}
	else
	{
		for (unsigned int i = 0; i < Win32Input::instance->mouse.size(); i++)
		{
			Win32Input::instance->mouse[i]->ToggleDefault(true);
		}
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
{ Destroy(); }
InputObject* Win32Input::CreateDevice(const SAIType inputType, Typedefs::WindowHandle targetApplication)
{
	if(!this->instance->targetHwin)
	{
		RECT rc;
		GetClientRect((HWND)targetApplication, &rc);
		
		AdjustWindowRect(&rc, GetWindowStyle((HWND)targetApplication), FALSE);
		
		rc.right  = rc.right - rc.left;
		rc.bottom = rc.bottom - rc.top;

		this->targetHwin = CreateWindowExW(  0, L"RawInputCallbackFunc" ,  NULL, NULL, rc.left, rc.top, rc.right, rc.bottom, 
											(HWND)targetApplication, NULL, (HINSTANCE)GetModuleHandle(0), NULL ); 
	}

	InputObject* val = 0;

	switch (inputType)
	{
		case SAIType_Keyboard:
		{
			Win32Keyboard* obj = new Win32Keyboard(this->targetHwin);
			if(!obj->Create())
			{
				delete obj;
				return 0;
			}

			this->keyboard.push_back(obj);
			val = obj;
		}
		break;

		case SAIType_Mouse:
		{
			Win32Mouse* obj = new Win32Mouse(this->targetHwin);
			if(!obj->Create())
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

	if(this->enabled)
	{
		for (unsigned int i = 0; i < this->mouse.size(); i++)
		{ this->mouse[i]->Deactivate(); }
		for (unsigned int i = 0; i < this->keyboard.size(); i++)
		{ this->keyboard[i]->Deactivate(); }
	}
	else
	{
		for (unsigned int i = 0; i < this->mouse.size(); i++)
		{ this->mouse[i]->Activate(); }
		for (unsigned int i = 0; i < this->keyboard.size(); i++)
		{ this->keyboard[i]->Activate(); }
	}
}
void Win32Input::Destroy ()
{
	ShowCursor(true);
	ClipCursor(0);

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


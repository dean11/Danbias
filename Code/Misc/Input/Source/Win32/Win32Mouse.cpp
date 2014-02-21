/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Input.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;

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


Win32Mouse::Win32Mouse(HWND target)
{
	this->device.usUsagePage = 0x01;
	this->device.hwndTarget = target;
	this->device.usUsage = RawInput_Usage_mouse;
	this->device.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;
	memset(&this->buttons[0], 0, sizeof(Buttons) * MAXBUTTONS);
}
Win32Mouse::~Win32Mouse()
{  }


bool Win32Mouse::IsBtnUp(Enum::SAMI btn) const
{
	if(btn == SAMI_Unknown) return false;

	return !this->buttons[btn].isDown;
}
bool Win32Mouse::IsBtnDown(Enum::SAMI btn) const
{
	if(btn == SAMI_Unknown) return false;

	return this->buttons[btn].isDown;
}
int Win32Mouse::GetWheelDelta() const
{
	return this->wheelDelta;
}
SAIPointInt2D& Win32Mouse::GetPixelPosition( Struct::SAIPointInt2D &targetMem ) const
{
	memcpy(&targetMem, &this->pixelPos, sizeof(SAIPointFloat2D));

	return targetMem;
}
SAIPointFloat2D& Win32Mouse::GetNormalizedPosition(Struct::SAIPointFloat2D& targetMem) const
{
	memcpy(&targetMem, &this->normalPos, sizeof(SAIPointFloat2D));

	return targetMem;
}
SAIPointFloat2D& Win32Mouse::GetDeltaPosition(Struct::SAIPointFloat2D& targetMem) const
{
	memcpy(&targetMem, &this->deltaPos, sizeof(SAIPointFloat2D));

	return targetMem;
}

void Win32Mouse::SetInputOptionType(Enum::InputOptionType options)
{
	if(! IsInputOptionSuported(options) ) return;

	this->inputMode = options;
	switch (options)
	{
		case Input::Enum::InputOptionType_RawInput:
		{
			if(!Create())
				printf("Warning! Could not create the specific device");
		}
		break;
		case Input::Enum::InputOptionType_PlatformDefault:
		{
			RAWINPUTDEVICE d;
			d.usUsagePage = 0x01;
			d.hwndTarget = 0;
			d.usUsage = RawInput_Usage_mouse;
			d.dwFlags = RIDEV_REMOVE;

			RegisterRawInputDevices(&d, 1, sizeof(RAWINPUTDEVICE));
		}
		break;
	}
}
bool Win32Mouse::IsInputOptionSuported(Enum::InputOptionType options) const
{
	switch (options)
	{
		case Input::Enum::InputOptionType_RawInput:			return true;
		case Input::Enum::InputOptionType_PlatformDefault:	return true;
	}
	return false;
}

void Win32Mouse::ProccessMouseData (RAWMOUSE mouse)
{
	bool isUp = true;
	Enum::SAMI btn = Enum::SAMI_Unknown;
	int delta = 0;
	Struct::SAIPointInt2D velocity;
	unsigned int makeCode = 0;
	MapButton(mouse, isUp, btn, delta, velocity, makeCode);

	if(velocity.Length() != 0)
	{
		this->pixelPos.x += velocity.x;
		this->pixelPos.y += velocity.y;

	//Contain
			 if(this->pixelPos.x < 0)					{ this->pixelPos.x = 0;						}
		else if(this->pixelPos.x > this->windowSize.x)	{ this->pixelPos.x = this->windowSize.x;	}
			 if(this->pixelPos.y < 0)					{ this->pixelPos.y = 0;						}
		else if(this->pixelPos.y > this->windowSize.y)	{ this->pixelPos.y = this->windowSize.y;	}

	
		RECT windowVertex;
		GetWindowRect( this->device.hwndTarget, &windowVertex );

		this->normalPos.x = (float)(pixelPos.x - windowVertex.left);
		this->normalPos.x /= (float)(windowVertex.right - windowVertex.left);

		this->normalPos.y = (float)(pixelPos.y - windowVertex.top);
		this->normalPos.y /= (float)(windowVertex.bottom - windowVertex.top);

		InternalOnMove(this->pixelPos);
	}

	if(delta != 0)
	{
		InternalOnScroll(delta);
	}


	if(btn == SAMI_Unknown) return;

	this->buttons[btn].isDown = !isUp;
	this->buttons[btn].makeCode = makeCode;

	//The btn is released.
	if(isUp)
	{
		InternalOnBtnRelease(btn);
	}
	//The btn is pressed.
	else
	{
		//The btn is down since last frame
		if(this->buttons[btn].isDown)
		{
			InternalOnBtnDown(btn);
		}
		else
		{
			InternalOnBtnPress(btn);
		}
	}
}
bool Win32Mouse::Create()
{
	if(!this->device.hwndTarget)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		windowSize.x = desktop.right;
		windowSize.y = desktop.bottom;
		this->device.dwFlags = 0;
	}
	else
	{
		RECT re;
		GetWindowRect(this->device.hwndTarget, &re);
		windowSize.x = re.right;
		windowSize.y = re.bottom;
	}

	if(RegisterRawInputDevices(&this->device, 1, sizeof(RAWINPUTDEVICE)) == TRUE)
	{
		return true;
	}

	return false;
}



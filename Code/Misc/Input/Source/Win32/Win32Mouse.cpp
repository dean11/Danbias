/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Mouse.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;

void MapButton(RAWMOUSE& rawMouse, bool &isUp, Enum::SAMI& btn, int& delta, Struct::SAIPoint2D& vel, unsigned int& mcode)
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


Win32Mouse::Win32Mouse()
{
	memset(&this->buttons[0], 0, sizeof(Buttons) * MAXBUTTONS);
}
Win32Mouse::~Win32Mouse()
{
}


bool Win32Mouse::IsBtnUp(Enum::SAMI btn)
{
	if(btn == SAMI_Unknown) return false;

	return !this->buttons[btn].isDown;
}
bool Win32Mouse::IsBtnDown(Enum::SAMI btn)
{
	if(btn == SAMI_Unknown) return false;

	return this->buttons[btn].isDown;
}

void Win32Mouse::ProccessMouseData (RAWMOUSE mouse)
{
	bool isUp = true;
	Enum::SAMI btn = Enum::SAMI_Unknown;
	int delta = 0;
	Struct::SAIPoint2D velocity;
	unsigned int makeCode = 0;
	MapButton(mouse, isUp, btn, delta, velocity, makeCode);

	if(velocity.Length() != 0)
	{
		this->pixelPos.x += this->deltaPos.x = velocity.x;
		this->pixelPos.y += this->deltaPos.y = velocity.y;
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
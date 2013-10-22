#include "InputController.h"
using namespace Oyster::Input;

namespace
{
	bool keys[256] = {0};
	bool prevs[256]= {0};
	bool mouse[5] = {0};
	bool mPrev[5] = {0};
	int XPos,YPos,PrevX,PrevY,DeltaX,DeltaY;
}

void Controller::KeyPressed(const WPARAM &Key)
{
	prevs[Key]=false;
	keys[Key]=true;
}

void Controller::KeyReleased(const WPARAM &Key)
{
	prevs[Key]=true;
	keys[Key] = false;
}

bool Controller::isKeyDown(const WPARAM &Key)
{
	return keys[Key];
}

bool Controller::isKeyPressed(const WPARAM &Key)
{
	if(keys[Key] && !prevs[Key])
	{
		prevs[Key] = keys[Key];
		return true;
	}
	return false;
}

bool Controller::isKeyReleased(const WPARAM &Key)
{
	if(!keys[Key] && prevs[Key])
	{
		prevs[Key] = keys[Key];
		return true;
	}
	return false;
}

void Controller::MouseBtnPressed(const WPARAM &btn)
{
	switch(btn)
	{
		case MK_LBUTTON:
			mouse[0] = true;
			mPrev[0] = false;
			break;
		case MK_RBUTTON:
			mouse[1] = true;
			mPrev[1] = false;
			break;
		case MK_MBUTTON:
			mouse[2] = true;
			mPrev[2] = false;
			break;
		case MK_XBUTTON1:
			mouse[3] = true;
			mPrev[3] = false;
			break;
		case MK_XBUTTON2:
			mouse[4] = true;
			mPrev[4] = false;
			break;
	}
}

void Controller::MouseBtnReleased(const WPARAM &btn)
{
	switch(btn)
	{
		case MK_LBUTTON:
			mouse[0] = false;
			mPrev[0] = true;
			break;
		case MK_RBUTTON:
			mouse[1] = false;
			mPrev[1] = true;
			break;
		case MK_MBUTTON:
			mouse[2] = false;
			mPrev[2] = true;
			break;
		case MK_XBUTTON1:
			mouse[3] = false;
			mPrev[3] = true;
			break;
		case MK_XBUTTON2:
			mouse[4] = false;
			mPrev[4] = true;
			break;
	}
}


bool Controller::isMouseBtnDown(const WPARAM &Btn)
{
	switch(Btn)
	{
		case MK_LBUTTON:
			return mouse[0];
		case MK_RBUTTON:
			return mouse[1];
		case MK_MBUTTON:
			return mouse[2];
		case MK_XBUTTON1:
			return mouse[3];
		case MK_XBUTTON2:
			return mouse[4];
	}

	return false;
}

bool Controller::isMouseBtnPressed(const WPARAM &Btn)
{
	switch(Btn)
	{
		case MK_LBUTTON:
			if(mouse[0] && !mPrev[0])
			{
				mPrev[0] = mouse[0];
				return true;
			}
			return false;
		case MK_RBUTTON:
			if(mouse[1] && !mPrev[1])
			{
				mPrev[1] = mouse[1];
				return true;
			}
			return false;
		case MK_MBUTTON:
			if(mouse[2] && !mPrev[2])
			{
				mPrev[2] = mouse[2];
				return true;
			}
			return false;
		case MK_XBUTTON1:
			if(mouse[3] && !mPrev[3])
			{
				mPrev[3] = mouse[3];
				return true;
			}
			return false;
		case MK_XBUTTON2:
			if(mouse[4] && !mPrev[4])
			{
				mPrev[4] = mouse[4];
				return true;
			}
			return false;
	}

	return false;
}

bool Controller::isMouseBtnReleased(const WPARAM &Btn)
{
	switch(Btn)
	{
		case MK_LBUTTON:
			if(!mouse[0] && mPrev[0])
			{
				mPrev[0] = mouse[0];
				return true;
			}
			return false;
		case MK_RBUTTON:
			if(!mouse[1] && mPrev[1])
			{
				mPrev[1] = mouse[1];
				return true;
			}
			return false;
		case MK_MBUTTON:
			if(!mouse[2] && mPrev[2])
			{
				mPrev[2] = mouse[2];
				return true;
			}
			return false;
		case MK_XBUTTON1:
			if(!mouse[3] && mPrev[3])
			{
				mPrev[3] = mouse[3];
				return true;
			}
			return false;
		case MK_XBUTTON2:
			if(!mouse[4] && mPrev[4])
			{
				mPrev[4] = mouse[4];
				return true;
			}
			return false;
	}

	return false;
}

void Controller::MouseMove(int x,int y)
{
	PrevY = YPos;
	PrevX = XPos;
	XPos = x;
	YPos = y;
	DeltaY = YPos - PrevY;
	DeltaX = XPos - PrevX;
}

int Controller::GetY()
{
	return YPos;
}

int Controller::GetX()
{
	return XPos;
}

float Controller::GetAnalogX()
{
	float f = (float)XPos;
	f /=( Window::Size.left/2);
	return f;
}

float Controller::GetAnalogY()
{
	float f = (float)YPos;
	f /=( Window::Size.top/2);
	return f;
}

int Controller::GetDeltaY()
{
	return DeltaY;
}

int Controller::GetDeltaX()
{
	return DeltaX;
}

void Controller::ResetDeltaX()
{
	DeltaX = 0;
}

void Controller::ResetDeltaY()
{
	DeltaY = 0;
}

void Controller::RestrictMouse(bool restrict)
{
	Oyster::Window::CaptureMouse(restrict);
}

//sets x=0,y=0 to be the center oc the client area
void Controller::OrigoCenter()
{
	int x = Window::Size.left/2;
	int y = Window::Size.top/2;

	Window::SetMouseOffset(-x,-y);
}

//default
void Controller::OrigoTopLeft()
{
	Oyster::Window::SetMouseOffset(0,0);
}
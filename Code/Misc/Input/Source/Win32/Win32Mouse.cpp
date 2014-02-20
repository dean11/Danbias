/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Input.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;


Win32Mouse::Win32Mouse()
{
	memset(&this->device, 0, sizeof(RAWINPUTDEVICE));
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
int Win32Mouse::GetWheelDelta()
{
	return this->wheelDelta;
}
Struct::SAIPointInt2D Win32Mouse::GetPixelPosition(Struct::SAIPointInt2D targetMem)
{
	targetMem = this->pixelPos;
	return targetMem;
}
Struct::SAIPointFloat2D	Win32Mouse::GetNormalizedPosition(Struct::SAIPointFloat2D targetMem)
{
	POINT mousePos;
	GetCursorPos( &mousePos );
	
	RECT windowVertex;
	GetWindowRect( this->device.hwndTarget, &windowVertex );
	
	this->normalPos.x = (float)(mousePos.x - windowVertex.left);
	this->normalPos.x /= (float)(windowVertex.right - windowVertex.left);
	
	this->normalPos.y = (float)(mousePos.y - windowVertex.top);
	this->normalPos.y /= (float)(windowVertex.bottom - windowVertex.top);

	return targetMem;
}

void Win32Mouse::ProccessMouseData (bool isUp, Enum::SAMI btn, int delta, Struct::SAIPointInt2D velocity, unsigned int makeCode)
{
	if(velocity.Length() != 0)
	{
		this->pixelPos.x += velocity.x;
		this->pixelPos.y += velocity.y;

		for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
		{
			if(this->mouseSubscribers[i])
				this->mouseSubscribers[i]->OnMouseMove(this->pixelPos, this);
		}
		MouseCallbackList *w = this->callbackList;
		while (w)
		{
			if(w->function)
				if (w->type == MouseCallbackList::CallbackDataType_OnMove)
					w->function.mouseMoveCallback(this->pixelPos, this);
			w = w->next;
		}
	}

	if(delta != 0)
	{
		for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
		{
			if(this->mouseSubscribers[i])
				this->mouseSubscribers[i]->OnMouseScroll(delta, this);
		}
		MouseCallbackList *w = this->callbackList;
		while (w)
		{
			if(w->function)
				if (w->type == MouseCallbackList::CallbackDataType_OnMove)
					w->function.mouseScrollCallback(delta, this);
			w = w->next;
		}
	}


	if(btn == SAMI_Unknown) return;

	this->buttons[btn].isDown = !isUp;
	this->buttons[btn].makeCode = makeCode;

	//The btn is released.
	if(isUp)
	{
		for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
		{
			if(this->mouseSubscribers[i])
				this->mouseSubscribers[i]->OnMouseRelease(btn, this);
		}
		MouseCallbackList *w = this->callbackList;
		while (w)
		{
			if(w->function)
				if (w->type == MouseCallbackList::CallbackDataType_OnRelease)
					w->function.mouseReleaseCallback(btn, this);
			w = w->next;
		}
	}
	//The btn is pressed.
	else
	{
		if(this->buttons[btn].isDown)
		{
			for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
			{
				if(this->mouseSubscribers[i])
					this->mouseSubscribers[i]->OnMouseDown(btn, this);
			}
			MouseCallbackList *w = this->callbackList;
			while (w)
			{
				if(w->function)
					if (w->type == MouseCallbackList::CallbackDataType_OnDown)
						w->function.mouseDownCallback(btn, this);
				w = w->next;
			}
		}
		else
		{
			for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
			{
				if(this->mouseSubscribers[i])
					this->mouseSubscribers[i]->OnMousePress(btn, this);
			}
			MouseCallbackList *w = this->callbackList;
			while (w)
			{
				if(w->function)
					if (w->type == MouseCallbackList::CallbackDataType_OnPress)
						w->function.mousePressCallback(btn, this);
				w = w->next;
			}
		}
	}


}

bool Win32Mouse::Create(HWND target)
{
	this->device.usUsagePage = 0x01;
	this->device.hwndTarget = target;
	this->device.usUsage = Input::RawInput_Usage_mouse;
	this->device.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;

	if(RegisterRawInputDevices(&this->device, 1, sizeof(RAWINPUTDEVICE)) == TRUE)
	{
		return true;
	}

	memset(&this->device, 0, sizeof(RAWINPUTDEVICE));
	
	return false;
}








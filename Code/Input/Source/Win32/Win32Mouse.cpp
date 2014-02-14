/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Mouse.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;


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
int Win32Mouse::GetWheelDelta()
{
	return this->wheelDelta;
}
Struct::SAIPoint2D Win32Mouse::GetPixelPosition(Struct::SAIPoint2D targetMem)
{
	targetMem = this->pixelPos;
	return targetMem;
}

void Win32Mouse::ProccessMouseData (bool isUp, Enum::SAMI btn, int delta, Struct::SAIPoint2D velocity, unsigned int makeCode)
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
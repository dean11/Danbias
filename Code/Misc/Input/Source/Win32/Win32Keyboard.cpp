/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\..\Include\Win32\Win32Keyboard.h"

#pragma warning ( disable : 4172 )
using namespace Input;
using namespace Input::Enum;

Win32Keyboard::Win32Keyboard()
{
	memset(&this->keys[0], 0, sizeof(Win32Keyboard::Keys) * MAXKEYS);
}
Win32Keyboard::~Win32Keyboard()
{

}

bool Win32Keyboard::IsKeyUp (SAKI key)
{
	return !this->keys[key].isDown;
}
bool Win32Keyboard::IsKeyDown (SAKI key)
{
	return this->keys[key].isDown;
}
const wchar_t* Win32Keyboard::GetAsText(Enum::SAKI key)
{
	if(Enum::SAKI_Unknown == key) return 0;

	// getting a human-readable string
	UINT temp = (this->keys[key].makecode << 16) | (this->keys[key].isE0 << 24);
	wchar_t buff[56] = {0};
	GetKeyNameTextW((LONG)temp, buff, 64);
	return buff;
}
void Win32Keyboard::ProccessKeyboardData (bool isUp, SAKI key, unsigned int makeCode, bool isE0)
{
	if(key == SAKI_Unknown) return;
	//The key is released.
	if(isUp)/*(k.Flags == RI_KEY_BREAK || k.Flags == (RI_KEY_BREAK | RI_KEY_E0) || k.Flags == (RI_KEY_BREAK | RI_KEY_E1))*/
	{
		if(key == SAKI_LeftAlt)
		{  }
		else if(key == SAKI_LeftCtrl)
		{}
		else if(key == SAKI_LeftShift)
		{}
		else if(key == SAKI_RightAlt)
		{}
		else if(key == SAKI_RightCtrl)
		{}
		else if(key == SAKI_RightShift)
		{}
		else
		{
			this->keys[key].isDown = false;
			this->keys[key].isE0 = isE0;
			this->keys[key].makecode = makeCode;
			for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
			{
				if(this->keyEventSubscrivers[i])
				{
					this->keyEventSubscrivers[i]->OnKeyRelease(key, GetAsText(key), this);
				}
			}
			KeyboardCallbackList *w = this->callbackList;
			while (w)
			{
				if(w->function)
					if (w->type == KeyboardCallbackList::CallbackDataType_OnRelease)
						w->function.keyReleaseCallback(key, GetAsText(key), this);
				w = w->next;
			}
		}
		
		//this->_procCollection.kd.key = (RIK)k.VKey;
		//this->_procCollection.kd.released = true;
	}
	//The key is pressed.
	else /*if (k.Flags == RI_KEY_MAKE || k.Flags == (RI_KEY_MAKE | RI_KEY_E0) || k.Flags == (RI_KEY_MAKE | RI_KEY_E1))*/
	{
		if(key == SAKI_LeftAlt)
		{}
		else if(key == SAKI_LeftCtrl)
		{}
		else if(key == SAKI_LeftShift)
		{}
		else if(key == SAKI_RightAlt)
		{}
		else if(key == SAKI_RightCtrl)
		{}
		else if(key == SAKI_RightShift)
		{}
		else
		{
			if(this->keys[key].isDown)
			{
				for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
				{
					if(this->keyEventSubscrivers[i])
					{
						this->keyEventSubscrivers[i]->OnKeyDown(key, GetAsText(key), this);
					}
				}
				KeyboardCallbackList *w = this->callbackList;
				while (w)
				{
					if(w->function)
						if (w->type == KeyboardCallbackList::CallbackDataType_OnDown)
							w->function.keyDownCallback(key, GetAsText(key), this);
					w = w->next;
				}
			}
			else
			{
				this->keys[key].isDown = true;
				this->keys[key].isE0 = isE0;
				this->keys[key].makecode = makeCode;
				for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
				{
					if(this->keyEventSubscrivers[i])
					{
						this->keyEventSubscrivers[i]->OnKeyPress(key, GetAsText(key), this);
					}
				}
				KeyboardCallbackList *w = this->callbackList;
				while (w)
				{
					if(w->function)
						if (w->type == KeyboardCallbackList::CallbackDataType_OnPress)
							w->function.keyPressCallback(key, GetAsText(key), this);
					w = w->next;
				}
			}
		}
	}
}

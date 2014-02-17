/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\Include\Keyboard.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Typedefs;
using namespace Input::Struct;


void Keyboard::ClearList(Keyboard::KeyboardCallbackList* first)
{
	KeyboardCallbackList* w = first;
	KeyboardCallbackList* removee = 0;

	while (w)
	{
		removee = w;
		w = w->next;
		delete removee;
	}
}
void Keyboard::AddToList(Keyboard::KeyboardCallbackList* first, KeyboardCallbackList::CallbackData data, KeyboardCallbackList::CallbackDataType type)
{
	KeyboardCallbackList *w = first;
	KeyboardCallbackList *prev = first;
	while (w)
	{ prev = w; w = w->next; }

	KeyboardCallbackList::CallbackData f;
	f = data;

	prev->next = new KeyboardCallbackList(f, type);
}
void Keyboard::RemoveFromList(KeyboardCallbackList* first, KeyboardCallbackList::CallbackData data)
{
	KeyboardCallbackList *w = first;
	KeyboardCallbackList *prev = first;
	while (w)
	{
		if(data == w->function)
		{
			KeyboardCallbackList *removee = w;
			w = w->next;
			prev->next = w;
			delete removee;
			break;
		}
		prev = w;
		w = w->next; 
	}
}
bool Keyboard::ExistsInList(KeyboardCallbackList* first, KeyboardCallbackList::CallbackData data)
{
	KeyboardCallbackList *w = first;
	while (w)
	{
		if(data == w->function)
		{
			return true;
		}
		w = w->next; 
	}
	return true;
}
bool Keyboard::ExistsInList(std::vector<KeyboardEvent*>& list, KeyboardEvent* data)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if(list[i] == data)
			return true;
	}
	return false;
}

Keyboard::Keyboard()
	:	InputObject(SAIType_Keyboard)
	,	callbackList(0)
{}
Keyboard::~Keyboard()
{

}

void Keyboard::AddOnKeyPressCallback (OnKeyPressCallback func)
{
	KeyboardCallbackList::CallbackData d;
	d.keyPressCallback = func;
	if(!this->callbackList) this->callbackList = new KeyboardCallbackList(d, KeyboardCallbackList::CallbackDataType_OnPress);
	else					AddToList(this->callbackList, d, KeyboardCallbackList::CallbackDataType_OnPress);
}
void Keyboard::AddOnKeyDownCallback (OnKeyDownCallback func)
{
	KeyboardCallbackList::CallbackData d;
	d.keyDownCallback = func;
	if(!this->callbackList) this->callbackList = new KeyboardCallbackList(d, KeyboardCallbackList::CallbackDataType_OnDown);
	else					AddToList(this->callbackList, d, KeyboardCallbackList::CallbackDataType_OnDown);
}
void Keyboard::AddOnKeyReleaseCallback (OnKeyReleaseCallback func)
{
	KeyboardCallbackList::CallbackData d;
	d.keyReleaseCallback = func;
	if(!this->callbackList) this->callbackList = new KeyboardCallbackList(d, KeyboardCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, KeyboardCallbackList::CallbackDataType_OnRelease);
}

void Keyboard::RemoveOnKeyPressCallback (OnKeyPressCallback func)
{
	RemoveFromList(this->callbackList, func);
}
void Keyboard::RemoveOnKeyDownCallback (OnKeyDownCallback func)
{
	RemoveFromList(this->callbackList, func);
}
void Keyboard::RemoveOnKeyReleaseCallback (OnKeyReleaseCallback func)
{
	RemoveFromList(this->callbackList, func);
}

void Keyboard::operator+= (KeyboardEvent* object)
{
	if(ExistsInList(this->keyEventSubscrivers, object)) return;

	this->keyEventSubscrivers.push_back(object);
}
void Keyboard::operator-= (KeyboardEvent* object)
{
	int i = -1;
	if((i = ExistsInList(this->keyEventSubscrivers, object)))
	{
		std::swap(this->keyEventSubscrivers[i], this->keyEventSubscrivers[this->keyEventSubscrivers.size() - 1]);
		this->keyEventSubscrivers.resize(this->keyEventSubscrivers.size() - 1);
	}
}


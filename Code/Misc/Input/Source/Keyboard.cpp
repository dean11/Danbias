/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\Include\Keyboard.h"

using namespace Input;
using namespace Input::Enum;
using namespace Input::Typedefs;
using namespace Input::Struct;

struct Keyboard::KeyboardCallbackList
{
	enum CallbackDataType
	{
		CallbackDataType_OnPress,
		CallbackDataType_OnDown,
		CallbackDataType_OnRelease
	} type;
	union CallbackData
	{
		Typedefs::OnKeyPressCallback	keyPressCallback;
		Typedefs::OnKeyDownCallback		keyDownCallback;
		Typedefs::OnKeyReleaseCallback	keyReleaseCallback;

		CallbackData (){ memset(this, 0, sizeof(CallbackData)); }
		CallbackData (Typedefs::OnKeyPressCallback o){ keyPressCallback = o; }
		bool operator ==(CallbackData o){ return o.keyDownCallback == keyDownCallback; }
		bool operator ==(Typedefs::OnKeyPressCallback o ){ return o == keyPressCallback; }
		operator bool(){ return this->keyDownCallback != 0; }
	} function;
	KeyboardCallbackList *next;
	KeyboardCallbackList(CallbackData func, CallbackDataType t) :function(func), next(0), type(t) { }
};

void ClearList(Keyboard::KeyboardCallbackList* first)
{
	Keyboard::KeyboardCallbackList* w = first;
	Keyboard::KeyboardCallbackList* removee = 0;

	while (w)
	{
		removee = w;
		w = w->next;
		delete removee;
	}
}
void AddToList(Keyboard::KeyboardCallbackList* first, Keyboard::KeyboardCallbackList::CallbackData data, Keyboard::KeyboardCallbackList::CallbackDataType type)
{
	Keyboard::KeyboardCallbackList *w = first;
	Keyboard::KeyboardCallbackList *prev = first;
	while (w)
	{ prev = w; w = w->next; }

	Keyboard::KeyboardCallbackList::CallbackData f;
	f = data;

	prev->next = new Keyboard::KeyboardCallbackList(f, type);
}
void RemoveFromList(Keyboard::KeyboardCallbackList* first, Keyboard::KeyboardCallbackList::CallbackData data)
{
	Keyboard::KeyboardCallbackList *w = first;
	Keyboard::KeyboardCallbackList *prev = first;
	while (w)
	{
		if(data == w->function)
		{
			Keyboard::KeyboardCallbackList *removee = w;
			w = w->next;
			prev->next = w;
			delete removee;
			break;
		}
		prev = w;
		w = w->next; 
	}
}
bool ExistsInList(Keyboard::KeyboardCallbackList* first, Keyboard::KeyboardCallbackList::CallbackData data)
{
	Keyboard::KeyboardCallbackList *w = first;
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
bool ExistsInList(std::vector<Keyboard::KeyboardEvent*>& list, Keyboard::KeyboardEvent* data)
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
	,	active(1)
	,	textTarget(0)
	,	writePos(0)
{}
Keyboard::~Keyboard()
{

}

void Keyboard::InternalOnKeyPress(Enum::SAKI key)
{
	for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
	{
		if(this->keyEventSubscrivers[i])
		{
			this->keyEventSubscrivers[i]->OnKeyPress(key, this);
		}
	}
	KeyboardCallbackList *w = this->callbackList;
	while (w)
	{
		if(w->function)
			if (w->type == KeyboardCallbackList::CallbackDataType_OnPress)
				w->function.keyPressCallback(key, this);
		w = w->next;
	}
}
void Keyboard::InternalOnKeyDown(Enum::SAKI key)
{
	for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
	{
		if(this->keyEventSubscrivers[i])
		{
			this->keyEventSubscrivers[i]->OnKeyDown(key, this);
		}
	}
	KeyboardCallbackList *w = this->callbackList;
	while (w)
	{
		if(w->function)
			if (w->type == KeyboardCallbackList::CallbackDataType_OnDown)
				w->function.keyDownCallback(key, this);
		w = w->next;
	}
}
void Keyboard::InternalOnKeyRelease(Enum::SAKI key)
{
	for (unsigned int i = 0; i < this->keyEventSubscrivers.size(); i++)
	{
		if(this->keyEventSubscrivers[i])
		{
			this->keyEventSubscrivers[i]->OnKeyRelease(key, this);
		}
	}
	KeyboardCallbackList *w = this->callbackList;
	while (w)
	{
		if(w->function)
			if (w->type == KeyboardCallbackList::CallbackDataType_OnRelease)
				w->function.keyReleaseCallback(key, this);
		w = w->next;
	}
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

void Keyboard::BindTextTarget( ::std::wstring *field )
{
	this->textTarget = field;
	
	if( field )
	{
		this->writePos = field->size();
	}
	else
	{
		this->writePos = 0;
	}
}
void Keyboard::ReleaseTextTarget( )
{
	this->BindTextTarget( nullptr );
}


/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\Include\Mouse.h"
#include <vector>

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;

struct Mouse::MouseCallbackList
{
	enum CallbackDataType
	{
		CallbackDataType_OnPress,
		CallbackDataType_OnDown,
		CallbackDataType_OnRelease,
		CallbackDataType_OnMove,
		CallbackDataType_OnScroll,
	} type;
	union CallbackData
	{
		Typedefs::OnMousePressCallback mousePressCallback;
		Typedefs::OnMouseDownCallback mouseDownCallback;
		Typedefs::OnMouseReleaseCallback mouseReleaseCallback;
		Typedefs::OnMouseMoveCallback mouseMoveCallback;
		Typedefs::OnMouseScrollCallback mouseScrollCallback;
		void* dummy;

		CallbackData (){ memset(this, 0, sizeof(CallbackData)); }
		CallbackData (void* d){ dummy = d; }
		bool operator ==(CallbackData o){ return o.dummy == dummy; }
		bool operator ==(void* o ){ return o == dummy; }
		operator bool(){ return this->dummy != 0; }
	} function;
	MouseCallbackList *next;
	MouseCallbackList(CallbackData func, CallbackDataType t) :function(func), next(0), type(t) { }
};

void ClearList(Mouse::MouseCallbackList* first)
{
	Mouse::MouseCallbackList* w = first;
	Mouse::MouseCallbackList* removee = 0;

	while (w)
	{
		removee = w;
		w = w->next;
		delete removee;
	}
}
void AddToList(Mouse::MouseCallbackList* first, Mouse::MouseCallbackList::CallbackData data, Mouse::MouseCallbackList::CallbackDataType type)
{
	Mouse::MouseCallbackList *w = first;
	Mouse::MouseCallbackList *prev = first;
	while (w)
	{ prev = w; w = w->next; }

	Mouse::MouseCallbackList::CallbackData f;
	f = data;

	prev->next = new Mouse::MouseCallbackList(f, type);
}
void RemoveFromList(Mouse::MouseCallbackList* first, Mouse::MouseCallbackList::CallbackData data)
{
	Mouse::MouseCallbackList *w = first;
	Mouse::MouseCallbackList *prev = first;
	while (w)
	{
		if(data == w->function)
		{
			Mouse::MouseCallbackList *removee = w;
			w = w->next;
			prev->next = w;
			delete removee;
			break;
		}
		prev = w;
		w = w->next; 
	}
}
bool ExistsInList(Mouse::MouseCallbackList* first, Mouse::MouseCallbackList::CallbackData data)
{
	Mouse::MouseCallbackList *w = first;
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
bool ExistsInList(std::vector<Mouse::MouseEvent*>& list, Mouse::MouseEvent* data)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if(list[i] == data)
			return true;
	}
	return false;
}

Mouse::Mouse()
	:	InputObject(Input::Enum::SAIType_Mouse)
	,	callbackList(0)
	,	wheelDelta(0)
	,	isCurorLocked(0)
	,	pixelPos()
	,	deltaPos()
{
}
Mouse::~Mouse()
{

}

void Mouse::InternalOnBtnPress(Enum::SAMI btn)
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
void Mouse::InternalOnBtnDown(Enum::SAMI btn)
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
void Mouse::InternalOnBtnRelease(Enum::SAMI btn)
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
void Mouse::InternalOnMove(Struct::SAIPoint2D cord)
{
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
void Mouse::InternalOnScroll(int delta)
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



int Mouse::GetWheelDelta() const
{
	return this->wheelDelta;
}
SAIPoint2D & Mouse::GetPixelPosition( Struct::SAIPoint2D &targetMem ) const
{
	targetMem.x = this->pixelPos.x;
	targetMem.y = this->pixelPos.y;
	return targetMem;
}
SAIPoint2D & Mouse::GetDeltaPosition( Struct::SAIPoint2D &targetMem ) const
{
	targetMem.x = this->deltaPos.x;
	targetMem.y = this->deltaPos.y;
	return targetMem;
}

void Mouse::AddOnMousePressCallback( Typedefs::OnMousePressCallback func)
{
	MouseCallbackList::CallbackData d;
	d.mousePressCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease);
}
void Mouse::AddOnMouseDownCallback( Typedefs::OnMouseDownCallback func )
{
	MouseCallbackList::CallbackData d;
	d.mouseDownCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease);
}
void Mouse::AddOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func )
{
	MouseCallbackList::CallbackData d;
	d.mouseReleaseCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease);
}
void Mouse::AddOnMouseMoveCallback( Typedefs::OnMouseMoveCallback func )
{
	MouseCallbackList::CallbackData d;
	d.mouseMoveCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease);
}
void Mouse::AddOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func )
{
	MouseCallbackList::CallbackData d;
	d.mouseScrollCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease);
}

void Mouse::RemoveOnMousePressCallback( Typedefs::OnMousePressCallback func)
{
	RemoveFromList(this->callbackList, func);
}
void Mouse::RemoveOnMouseDownCallback( Typedefs::OnMouseDownCallback func )
{
	RemoveFromList(this->callbackList, func);
}
void Mouse::RemoveOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func )
{
	RemoveFromList(this->callbackList, func);
}
void Mouse::RemoveOnMouseMoveCallback( Typedefs::OnMouseMoveCallback func )
{
	RemoveFromList(this->callbackList, func);
}
void Mouse::RemoveOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func )
{
	RemoveFromList(this->callbackList, func);
}

void Mouse::SetPixelPos(int x, int y)
{
	this->pixelPos.x = x;
	this->pixelPos.y = y;
}
void Mouse::ToggleCursor(bool toggler)
{
	this->isCurorLocked = toggler;
}

void Mouse::operator+= (MouseEvent* object)
{
	if(ExistsInList(this->mouseSubscribers, object)) return;

	this->mouseSubscribers.push_back(object);
}
void Mouse::operator-= (MouseEvent* object)
{
	int i = -1;
	if((i = ExistsInList(this->mouseSubscribers, object)))
	{
		std::swap(this->mouseSubscribers[i], this->mouseSubscribers[this->mouseSubscribers.size() - 1]);
		this->mouseSubscribers.resize(this->mouseSubscribers.size() - 1);
	}
}





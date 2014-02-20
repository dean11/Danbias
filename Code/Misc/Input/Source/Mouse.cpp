/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#include "..\Include\Mouse.h"
#include <vector>

using namespace Input;
using namespace Input::Enum;
using namespace Input::Struct;
using namespace Input::Typedefs;


void Mouse::ClearList(Mouse::MouseCallbackList* first)
{
	MouseCallbackList* w = first;
	MouseCallbackList* removee = 0;

	while (w)
	{
		removee = w;
		w = w->next;
		delete removee;
	}
}
void Mouse::AddToList(Mouse::MouseCallbackList* first, MouseCallbackList::CallbackData data, MouseCallbackList::CallbackDataType type)
{
	MouseCallbackList *w = first;
	MouseCallbackList *prev = first;
	while (w)
	{ prev = w; w = w->next; }

	MouseCallbackList::CallbackData f;
	f = data;

	prev->next = new MouseCallbackList(f, type);
}
void Mouse::RemoveFromList(MouseCallbackList* first, MouseCallbackList::CallbackData data)
{
	MouseCallbackList *w = first;
	MouseCallbackList *prev = first;
	while (w)
	{
		if(data == w->function)
		{
			MouseCallbackList *removee = w;
			w = w->next;
			prev->next = w;
			delete removee;
			break;
		}
		prev = w;
		w = w->next; 
	}
}
bool Mouse::ExistsInList(MouseCallbackList* first, MouseCallbackList::CallbackData data)
{
	MouseCallbackList *w = first;
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
bool Mouse::ExistsInList(std::vector<MouseEvent*>& list, MouseEvent* data)
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





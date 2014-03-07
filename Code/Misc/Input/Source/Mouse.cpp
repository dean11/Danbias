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
		CallbackDataType_OnEvent,
		CallbackDataType_OnPress,
		CallbackDataType_OnDown,
		CallbackDataType_OnRelease,
		CallbackDataType_OnMovePixelPos,
		CallbackDataType_OnMoveVelocity,
		CallbackDataType_OnScroll,
	} type;
	union CallbackData
	{
		Typedefs::OnMouseCallback mouseCallback;
		Typedefs::OnMousePressCallback mousePressCallback;
		Typedefs::OnMouseDownCallback mouseDownCallback;
		Typedefs::OnMouseReleaseCallback mouseReleaseCallback;
		Typedefs::OnMouseMovePixelPosCallback mouseMovePixelPosCallback;
		Typedefs::OnMouseMoveVelocityCallback mouseMoveVelocityCallback;
		Typedefs::OnMouseScrollCallback mouseScrollCallback;
		void* dummy;

		CallbackData (){ memset(this, 0, sizeof(CallbackData)); }
		CallbackData (void* d){ dummy = d; }
		bool operator ==(CallbackData o){ return o.dummy == dummy; }
		bool operator ==(void* o ){ return o == dummy; }
		operator bool(){ return this->dummy != 0; }
	} function;
	MouseCallbackList *next;
	void* tag;
	MouseCallbackList(CallbackData func, CallbackDataType t, void* ct) :function(func), next(0), type(t), tag(ct) { }
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
void AddToList(Mouse::MouseCallbackList* first, Mouse::MouseCallbackList::CallbackData data, Mouse::MouseCallbackList::CallbackDataType type, void* tag)
{
	Mouse::MouseCallbackList *w = first;
	Mouse::MouseCallbackList *prev = first;
	while (w)
	{ prev = w; w = w->next; }

	Mouse::MouseCallbackList::CallbackData f;
	f = data;

	prev->next = new Mouse::MouseCallbackList(f, type, tag);
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
void RemoveFromList(std::vector<Mouse::MouseEvent*>& list, Mouse::MouseEvent* data)
{
	for (unsigned int i = 0; i < list.size(); i++)
	{
		if(list[i] == data)
		{
			std::swap(list[i], list[list.size() - 1]);
			list.resize(list.size() - 1);
			return;
		}
	}
}

Mouse::Mouse()
	:	InputObject(Input::Enum::SAIType_Mouse)
	,	callbackList(0)
	,	wheelDelta(0)
	,	isCurorLocked(0)
	,	pixelPos()
	,	deltaPos()
	,	sensitivity(1.0f)
{
}
Mouse::~Mouse()
{

}

void Mouse::InternalOnEvent(MouseEventData & data)
{
	for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
	{
		if(this->mouseSubscribers[i])
			this->mouseSubscribers[i]->OnMouse(data);
	}
	MouseCallbackList *w = this->callbackList;
	while (w)
	{
		if(w->function)
			if (w->type == MouseCallbackList::CallbackDataType_OnEvent)
			{
				data.tag = w->tag;
				w->function.mouseCallback(data);
			}
		w = w->next;
	}
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
void Mouse::InternalOnMove(Struct::SAIPointInt2D pixelPos, Struct::SAIPointFloat2D velocity)
{
	for (unsigned int i = 0; i < this->mouseSubscribers.size(); i++)
	{
		if(this->mouseSubscribers[i])
		{
			this->mouseSubscribers[i]->OnMouseMovePixelPos(pixelPos, this);
			this->mouseSubscribers[i]->OnMouseMoveVelocity(velocity, this);
		}
	}
	MouseCallbackList *w = this->callbackList;
	while (w)
	{
		if(w->function)
			if (w->type == MouseCallbackList::CallbackDataType_OnMovePixelPos)
				w->function.mouseMovePixelPosCallback(pixelPos, this);
			else if (w->type == MouseCallbackList::CallbackDataType_OnMoveVelocity)
				w->function.mouseMoveVelocityCallback(velocity, this);
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
			if (w->type == MouseCallbackList::CallbackDataType_OnScroll)
				w->function.mouseScrollCallback(delta, this);
		w = w->next;
	}
}

void Mouse::AddMouseEvent(MouseEvent* object)
{
	if(ExistsInList(this->mouseSubscribers, object)) return;

	this->mouseSubscribers.push_back(object);
}
void Mouse::RemoveMouseEvent(MouseEvent* object)
{
	RemoveFromList(this->mouseSubscribers, object);
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

void Mouse::AddOnMouseCallback( Typedefs::OnMouseCallback func, void* tag)
{
	MouseCallbackList::CallbackData d;
	d.mouseCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnEvent, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnEvent, tag);
}
void Mouse::AddOnMousePressCallback( Typedefs::OnMousePressCallback func, void* tag)
{
	MouseCallbackList::CallbackData d;
	d.mousePressCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnPress, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnPress, tag);
}
void Mouse::AddOnMouseDownCallback( Typedefs::OnMouseDownCallback func, void* tag )
{
	MouseCallbackList::CallbackData d;
	d.mouseDownCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnDown, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnDown, tag);
}
void Mouse::AddOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func, void* tag )
{
	MouseCallbackList::CallbackData d;
	d.mouseReleaseCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnRelease, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnRelease, tag);
}
void Mouse::AddOnMouseMovePixelPosCallback( Typedefs::OnMouseMovePixelPosCallback func, void* tag )
{
	MouseCallbackList::CallbackData d;
	d.mouseMovePixelPosCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnMovePixelPos, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnMovePixelPos, tag);
}
void Mouse::AddOnMouseMoveVelocityCallback( Typedefs::OnMouseMoveVelocityCallback func, void* tag )
{
	MouseCallbackList::CallbackData d;
	d.mouseMoveVelocityCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnMoveVelocity, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnMoveVelocity, tag);
}
void Mouse::AddOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func, void* tag )
{
	MouseCallbackList::CallbackData d;
	d.mouseScrollCallback = func;
	if(!this->callbackList) this->callbackList = new MouseCallbackList(d, MouseCallbackList::CallbackDataType_OnScroll, tag);
	else					AddToList(this->callbackList, d, MouseCallbackList::CallbackDataType_OnScroll, tag);
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
void Mouse::RemoveOnMouseMovePixelPosCallback( Typedefs::OnMouseMovePixelPosCallback func )
{
	RemoveFromList(this->callbackList, func);
}
void Mouse::RemoveOnMouseMoveVelocityCallback( Typedefs::OnMouseMoveVelocityCallback func )
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
void Mouse::SetSensitivity(float value)
{
	this->sensitivity = value;
}
float Mouse::GetSensitivity() const
{
	return this->sensitivity;
}






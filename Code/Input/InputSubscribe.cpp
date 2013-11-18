#include "RawInput.h"


template<typename FNC, typename PARAM, typename OBJECT_PARAM>
void SUBSCRIBE(SubscribeList<FNC, PARAM>* l, FNC _fnc, OBJECT_PARAM p)
{
	SubscribeList<FNC, PARAM>* t = new SubscribeList<FNC, PARAM>();

	t->param = p;
	t->fnc = _fnc;
	t->next = l;
	l = t;
}
template<typename FNC, typename PARAM>
void UNSUBSCRIBE(SubscribeList<FNC, PARAM>* l, FNC fnc)
{
	SubscribeList<FNC, PARAM>* w = l;
	SubscribeList<FNC, PARAM>* p = 0;
	while (w)
	{
		if(w->fnc == fnc)
		{
			if(p)
				p->next = w->next;

			delete w;
			w = 0;
		}
		else
		{
			p = w;
			w = w->next;
		}
	}
}


void RawInput_Impl::Input_Subscribe		(IN INPUT_CALLBACK fnc)
{
	//SUBSCRIBE(this->_procInput, fnc, &this->_procCollection.id);
}
void RawInput_Impl::Input_Unsubscribe	(IN INPUT_CALLBACK fnc)		 
{
	//UNSUBSCRIBE(this->_procSystemKey, fnc);
}










/*
void RawInput::Subscribe	(RAW_KEY_PROC fnc)
{
	KeyboardProcList* t = new KeyboardProcList();
	t->fnc = fnc;
	t->next = 0;

	KeyboardProcList* w = this->_keyProcList;
	KeyboardProcList* prev = this->_keyProcList;

	while (w)
	{
		prev = w;
		w = w->next;
	}

	prev->next = t;
}
void RawInput::Subscribe	(RAW_MOUSE_PROC fnc)
{
	MouseProcList* t = new MouseProcList();
	t->fnc = fnc;
	t->next = 0;

	MouseProcList* w = this->_mouseProcList;
	MouseProcList* prev = this->_mouseProcList;

	while (w)
	{
		prev = w;
		w = w->next;
	}

	prev->next = t;
}
void RawInput::Unsubscribe	(RAW_KEY_PROC fnc)
{
	KeyboardProcList* w = this->_keyProcList;
	KeyboardProcList* prev = 0;

	while (w)
	{
		if(w->fnc == fnc)
		{
			if(prev)
				prev->next = w->next;

			delete w;
			w = 0;
		}
		else
		{
			prev = w;
			w = w->next;
		}
	}
}
void RawInput::Unsubscribe	(RAW_MOUSE_PROC fnc)
{
	MouseProcList* w = this->_mouseProcList;
	MouseProcList* prev = 0;

	while (w)
	{
		if(w->fnc == fnc)
		{
			if(prev)
				prev->next = w->next;

			delete w;
			w = 0;
		}
		else
		{
			prev = w;
			w = w->next;
		}
	}
}
*/








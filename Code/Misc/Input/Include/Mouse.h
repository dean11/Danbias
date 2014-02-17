/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_MOUSE_H
#define INPUT_MOUSE_H

#include "InputObject.h"
#include <vector>

namespace Input
{
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Enum
	{
		//! Contains keycodes (SAMI -> Sam Awsome Mouse Input)
		enum SAMI
		{
			SAMI_MouseLeftBtn,
			SAMI_MouseMiddleBtn,	
			SAMI_MouseRightBtn,
			SAMI_MouseBtnX01,
			SAMI_MouseBtnX02,
			SAMI_MouseBtnX03,
			SAMI_MouseBtnX04,
			SAMI_MouseBtnX05,
			SAMI_MouseBtnX06,
			SAMI_MouseBtnX07,
			SAMI_MouseBtnX08,
			SAMI_MouseBtnX09,
			SAMI_MouseBtnX10,
			SAMI_MouseBtnX11,
			SAMI_MouseBtnX12,
			SAMI_MouseBtnX13,
			SAMI_MouseBtnX14,
			SAMI_MouseBtnX15,
			SAMI_MouseBtnX16,
			SAMI_MouseBtnX17,
			SAMI_MouseBtnX18,
			SAMI_MouseBtnX19,
			SAMI_MouseBtnX20,
			SAMI_Unknown,
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Typedefs
	{
		typedef void(*OnMousePressCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseDownCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseReleaseCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseMoveCallback)(Struct::SAIPoint2D, Mouse* sender);
		typedef void(*OnMouseScrollCallback)(int delta, Mouse* sender);
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	
	class Mouse	:public InputObject
	{
	public:
		class MouseEvent
		{
		public:
			virtual void OnMousePress	( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseDown	( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseRelease	( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseMove	( Struct::SAIPoint2D coordinate, Mouse* sender )			{ }
			virtual void OnMouseScroll	( int delta, Mouse* sender )								{ }
		};
	
	public:
		virtual bool				IsBtnUp(Enum::SAMI key) = 0;
		virtual bool				IsBtnDown(Enum::SAMI key) = 0;
		virtual int					GetWheelDelta() = 0;
		virtual Struct::SAIPoint2D	GetPixelPosition(Struct::SAIPoint2D targetMem = Struct::SAIPoint2D()) = 0;

	public:
		void AddOnMousePressCallback( Typedefs::OnMousePressCallback func);
		void AddOnMouseDownCallback( Typedefs::OnMouseDownCallback func );
		void AddOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func );
		void AddOnMouseMoveCallback( Typedefs::OnMouseMoveCallback func );
		void AddOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func );

		void RemoveOnMousePressCallback( Typedefs::OnMousePressCallback func);
		void RemoveOnMouseDownCallback( Typedefs::OnMouseDownCallback func );
		void RemoveOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func );
		void RemoveOnMouseMoveCallback( Typedefs::OnMouseMoveCallback func );
		void RemoveOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func );

		void SetPixelPos(int x, int y);
		void ToggleCursor(bool toggler);

	private:
		void operator+= (MouseEvent* object);
		void operator-= (MouseEvent* object);

	protected:
		Mouse();
		~Mouse();

	protected:
		struct MouseCallbackList
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

	protected:
		void ClearList(MouseCallbackList* first);
		void AddToList(MouseCallbackList* first, MouseCallbackList::CallbackData data, MouseCallbackList::CallbackDataType type);
		void RemoveFromList(MouseCallbackList* first, MouseCallbackList::CallbackData data);
		bool ExistsInList(MouseCallbackList* first, MouseCallbackList::CallbackData data);
		bool ExistsInList(std::vector<MouseEvent*>& list, MouseEvent* data);

	protected:
		std::vector<MouseEvent*>	mouseSubscribers;
		MouseCallbackList*			callbackList;
		Struct::SAIPoint2D			pixelPos;
		bool						isCurorLocked;
		int							wheelDelta;
	};
}


#endif // !INPUT_MOUSE_H

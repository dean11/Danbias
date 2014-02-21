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
		typedef void(*OnMouseMoveCallback)(Struct::SAIPoint2D cord, Mouse* sender);
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
	
	public: /* Manual check functions */
		virtual ~Mouse();

		virtual bool				 IsBtnUp(Enum::SAMI key) = 0;
		virtual bool				 IsBtnDown(Enum::SAMI key) = 0;
		
	public: /* global subscribe callback functions */
		int					 GetWheelDelta() const;
		Struct::SAIPoint2D & GetPixelPosition( Struct::SAIPoint2D &targetMem = Struct::SAIPoint2D() ) const;
		Struct::SAIPoint2D & GetDeltaPosition( Struct::SAIPoint2D &targetMem = Struct::SAIPoint2D() ) const;

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

	public: /* From InputObject */
		virtual void Activate () override	{ this->active = true; }
		virtual void Deactivate () override	{ this->active = false; }
		virtual bool IsActive() override	{ return this->active; }

	public:
		void operator+= (MouseEvent* object);
		void operator-= (MouseEvent* object);

		void SetPixelPos(int x, int y);
		void ToggleCursor(bool toggler);

	public:
		struct MouseCallbackList;

	protected:
		Mouse();

	protected:
		void InternalOnBtnPress(Enum::SAMI key);
		void InternalOnBtnDown(Enum::SAMI key);
		void InternalOnBtnRelease(Enum::SAMI key);
		void InternalOnMove(Struct::SAIPoint2D cord);
		void InternalOnScroll(int delta);

	protected:
		std::vector<MouseEvent*>	mouseSubscribers;
		MouseCallbackList*			callbackList;
		Struct::SAIPoint2D			pixelPos, deltaPos;
		bool						isCurorLocked;
		int							wheelDelta;
		bool							active;
	};
}


#endif // !INPUT_MOUSE_H

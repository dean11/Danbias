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
			SAMI_MouseMove,
			SAMI_MouseScroll,
			SAMI_Unknown,
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Struct
	{
		struct MouseEventData
		{
			Enum::SAMI type;
			Enum::ButtonState buttonState;
			Struct::SAIPointInt2D pixelPos;
			Struct::SAIPointFloat2D normalizedPos;
			Struct::SAIPointFloat2D velocity;
			Mouse* sender;
			int scrollDelta;
			void* tag;
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Typedefs
	{
		typedef void(*OnMouseCallback)( const Struct::MouseEventData& eventData );
		typedef void(*OnMousePressCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseDownCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseReleaseCallback)(Enum::SAMI btn, Mouse* sender);
		typedef void(*OnMouseMovePixelPosCallback)(Struct::SAIPointInt2D cord, Mouse* sender);
		typedef void(*OnMouseMoveVelocityCallback)(Struct::SAIPointFloat2D cord, Mouse* sender);
		typedef void(*OnMouseScrollCallback)(int delta, Mouse* sender);
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	
	class Mouse	:public InputObject
	{
	public:

		class MouseEvent
		{
		public:
			virtual void OnMouse				( const Struct::MouseEventData& eventData )							{ }
			virtual void OnMousePress			( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseDown			( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseRelease			( Enum::SAMI key, Mouse* sender )							{ }
			virtual void OnMouseMovePixelPos	( Struct::SAIPointInt2D coordinate, Mouse* sender )			{ }
			virtual void OnMouseMoveVelocity	( Struct::SAIPointFloat2D coordinate, Mouse* sender )		{ }
			virtual void OnMouseScroll			( int delta, Mouse* sender )								{ }
		};
	
	public: /* Manual check functions */
		virtual bool						IsBtnUp(Enum::SAMI key) const = 0;
		virtual bool						IsBtnDown(Enum::SAMI key) const = 0;
		virtual int							GetWheelDelta() const = 0;
		virtual Struct::SAIPointInt2D&		GetPixelPosition(Struct::SAIPointInt2D& targetMem = Struct::SAIPointInt2D()) const = 0;
		virtual Struct::SAIPointFloat2D&	GetNormalizedPosition(Struct::SAIPointFloat2D& targetMem = Struct::SAIPointFloat2D()) = 0;
		virtual Struct::SAIPointFloat2D&	GetDeltaPosition(Struct::SAIPointFloat2D& targetMem = Struct::SAIPointFloat2D()) const = 0;

	public: /* From InputObject */
		virtual void Activate () override		= 0;
		virtual void Deactivate () override		= 0;
		virtual bool IsActive() override		= 0;

	public: /* global subscribe callback functions */
		void AddMouseEvent(MouseEvent* object);
		void RemoveMouseEvent(MouseEvent* object);
		void operator+= (MouseEvent* object);
		void operator-= (MouseEvent* object);

	public: /* global subscribe callback functions */
		void AddOnMouseCallback( Typedefs::OnMouseCallback func, void* tag);
		void AddOnMousePressCallback( Typedefs::OnMousePressCallback func, void* tag);
		void AddOnMouseDownCallback( Typedefs::OnMouseDownCallback func, void* tag );
		void AddOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func, void* tag );
		void AddOnMouseMovePixelPosCallback( Typedefs::OnMouseMovePixelPosCallback func, void* tag );
		void AddOnMouseMoveVelocityCallback( Typedefs::OnMouseMoveVelocityCallback func, void* tag );
		void AddOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func, void* tag );

		void RemoveOnMouseCallback( Typedefs::OnMouseCallback func);
		void RemoveOnMousePressCallback( Typedefs::OnMousePressCallback func);
		void RemoveOnMouseDownCallback( Typedefs::OnMouseDownCallback func );
		void RemoveOnMouseReleaseCallback( Typedefs::OnMouseReleaseCallback func );
		void RemoveOnMouseMovePixelPosCallback( Typedefs::OnMouseMovePixelPosCallback  func );
		void RemoveOnMouseMoveVelocityCallback( Typedefs::OnMouseMoveVelocityCallback  func );
		void RemoveOnMouseScrollCallback( Typedefs::OnMouseScrollCallback func );

		void SetPixelPos(int x, int y);
		void ToggleCursor(bool toggler);
		void SetSensitivity(float value);
		float GetSensitivity() const;

	public:
		struct MouseCallbackList;

	protected:
		Mouse();
		virtual ~Mouse();

	protected:
		void InternalOnEvent(Struct::MouseEventData & data);
		void InternalOnBtnPress(Enum::SAMI key);
		void InternalOnBtnDown(Enum::SAMI key);
		void InternalOnBtnRelease(Enum::SAMI key);
		void InternalOnMove(Struct::SAIPointInt2D pixelPos, Struct::SAIPointFloat2D velocity);
		void InternalOnScroll(int delta);

	protected:
		std::vector<MouseEvent*>	mouseSubscribers;
		MouseCallbackList*			callbackList;
		Struct::SAIPointFloat2D		pixelPos;
		Struct::SAIPointFloat2D		velocity;
		Struct::SAIPointFloat2D		normalPos;
		Struct::SAIPointFloat2D		deltaPos;
		
		bool						isCurorLocked;
		int							wheelDelta;
		float						sensitivity;
	};
}


#endif // !INPUT_MOUSE_H

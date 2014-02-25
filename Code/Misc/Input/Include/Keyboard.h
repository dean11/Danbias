/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_KEBOARD_H
#define INPUT_KEBOARD_H

#include "InputObject.h"
#include <vector>

namespace Input
{
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Enum
	{
		//! Contains keycodes (Enum::SAKI -> Super Awsome Key Input)
		enum SAKI
		{
			SAKI_Backspace			,
			SAKI_Tab				,
			SAKI_Enter				,
			SAKI_LeftShift			,
			SAKI_RightShift			,
			SAKI_LeftAlt			,
			SAKI_RightAlt			,
			SAKI_LeftCtrl			,
			SAKI_RightCtrl			,
			SAKI_Pause				,
			SAKI_CapsLock			,
			SAKI_Escape				,
			SAKI_Space				,
			SAKI_PgUp				,
			SAKI_PgDown				,
			SAKI_End				,
			SAKI_Home				,
			SAKI_Left				,
			SAKI_Up					,
			SAKI_Right				,
			SAKI_Down				,
			SAKI_PrintScreen		,
			SAKI_Insert				,
			SAKI_Delete				,
			SAKI_0					,
			SAKI_1					,
			SAKI_2					,
			SAKI_3					,
			SAKI_4					,
			SAKI_5					,
			SAKI_6					,
			SAKI_7					,
			SAKI_8					,
			SAKI_9					,
			SAKI_Add				,
			SAKI_Comma				,
			SAKI_Minus				,
			SAKI_Period				,
			SAKI_A					,
			SAKI_B					,
			SAKI_C					,
			SAKI_D					,
			SAKI_E					,
			SAKI_F					,
			SAKI_G					,
			SAKI_H					,
			SAKI_I					,
			SAKI_J					,
			SAKI_K					,
			SAKI_L					,
			SAKI_M					,
			SAKI_N					,
			SAKI_O					,
			SAKI_P					,
			SAKI_Q					,
			SAKI_R					,
			SAKI_S					,
			SAKI_T					,
			SAKI_U					,
			SAKI_V					,
			SAKI_W					,
			SAKI_X					,
			SAKI_Y					,
			SAKI_Z					,
			SAKI_LeftWindows		,
			SAKI_RightWindows		,
			SAKI_Numpad0			,
			SAKI_Numpad1			,
			SAKI_Numpad2			,
			SAKI_Numpad3			,
			SAKI_Numpad4			,
			SAKI_Numpad5			,
			SAKI_Numpad6			,
			SAKI_Numpad7			,
			SAKI_Numpad8			,
			SAKI_Numpad9			,
			SAKI_NumpadEnter		,
			SAKI_NumpadMultiply		,
			SAKI_NumpadPlus			,
			SAKI_NumpadSubtract		,
			SAKI_NumpadDecimal		,
			SAKI_NumpadDivide		,
			SAKI_F1					,
			SAKI_F2					,
			SAKI_F3					,
			SAKI_F4					,
			SAKI_F5					,
			SAKI_F6					,
			SAKI_F7					,
			SAKI_F8					,
			SAKI_F9					,
			SAKI_F10				,
			SAKI_F11				,
			SAKI_F12				,
			SAKI_F13				,
			SAKI_F14				,
			SAKI_F15				,
			SAKI_F16				,
			SAKI_F17				,
			SAKI_F18				,
			SAKI_F19				,
			SAKI_F20				,
			SAKI_F21				,
			SAKI_F22				,
			SAKI_F23				,
			SAKI_F24				,
			SAKI_Numlock			,
			SAKI_ScrlLock			,
			SAKI_VolumeMute,
			SAKI_VolumeDown,
			SAKI_VolumeUp,
			SAKI_MediaNext,
			SAKI_MediaPrev,
			SAKI_MediaStop,
			SAKI_MediaPlayPause,

			SAKI_Unknown
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Struct
	{
		struct KeyboardEventData
		{
			Enum::SAKI key;
			Enum::ButtonState state;
			Keyboard* sender;
			void* tag;
		};
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	namespace Typedefs
	{
		typedef void(*OnKeyEventCallback)	(const Struct::KeyboardEventData& eventData);
		typedef void(*OnKeyPressCallback)	(Enum::SAKI key, Keyboard* sender, void* tag);
		typedef void(*OnKeyDownCallback)	(Enum::SAKI key, Keyboard* sender, void* tag);
		typedef void(*OnKeyReleaseCallback)	(Enum::SAKI key, Keyboard* sender, void* tag);
	}
	//-----------------------------------------------------------------------------------------------------------------------------

	class Keyboard	:public InputObject
	{
	public:
		class KeyboardEvent
		{
		public:
			virtual void OnKeyEvent		(const Struct::KeyboardEventData& eventData)	{  }
			virtual void OnKeyPress		(Enum::SAKI key, Keyboard* sender)				{  }
			virtual void OnKeyDown		(Enum::SAKI key, Keyboard* sender)				{  }
			virtual void OnKeyRelease	(Enum::SAKI key, Keyboard* sender)				{  }
		};

	public: /* Manual check functions */
		virtual ~Keyboard();

		virtual bool IsKeyUp (Enum::SAKI key) = 0;
		virtual bool IsKeyDown (Enum::SAKI key) = 0;
		virtual wchar_t* GetAsText(Enum::SAKI key) = 0;

	public: /* From InputObject */
		virtual void Activate () override		= 0;
		virtual void Deactivate () override		= 0;
		virtual bool IsActive() override		= 0;

	public: /* object subscribe functions */
		void AddKeyboardEvent			(KeyboardEvent* object);
		void RemoveKeyboardEvent		(KeyboardEvent* object);
		void operator+=					(KeyboardEvent* object);
		void operator-=					(KeyboardEvent* object);

	public: /* global subscribe callback functions */
		void AddOnKeyEventCallback		(Typedefs::OnKeyEventCallback func, void* tag);
		void AddOnKeyPressCallback		(Typedefs::OnKeyPressCallback func, void* tag);
		void AddOnKeyDownCallback		(Typedefs::OnKeyDownCallback func, void* tag);
		void AddOnKeyReleaseCallback	(Typedefs::OnKeyReleaseCallback func, void* tag);

		void RemoveOnKeyEventCallback	(Typedefs::OnKeyEventCallback func);
		void RemoveOnKeyPressCallback	(Typedefs::OnKeyPressCallback func);
		void RemoveOnKeyDownCallback	(Typedefs::OnKeyDownCallback func);
		void RemoveOnKeyReleaseCallback (Typedefs::OnKeyReleaseCallback func);

	public:
		

		void BindTextTarget( ::std::wstring *field );
		void ReleaseTextTarget();

	public:
		struct KeyboardCallbackList;

	protected:
		Keyboard();

	protected: /* Internal event proc */
		void InternalOnEvent(Struct::KeyboardEventData& data);
		void InternalOnKeyPress(Enum::SAKI key);
		void InternalOnKeyDown(Enum::SAKI key);
		void InternalOnKeyRelease(Enum::SAKI key);

	protected:
		std::vector<KeyboardEvent*>		keyEventSubscrivers;
		KeyboardCallbackList*			callbackList;
		::std::wstring*					textTarget;
		::std::wstring::size_type		writePos;
		bool							active;
	};
}

#endif // !INPUT_KEBOARD_H

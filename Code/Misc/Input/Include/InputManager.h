/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef RAW_INPUT_H
#define RAW_INPUT_H

#include "PreReq.h"
#include "Common.h"
#include "InputObject.h"
#include "Mouse.h"
#include "Keyboard.h"

namespace Input
{
	class InputManager
	{
		public:
			/**
			* @return Returns an instance of the default input manager.
			*/
			static InputManager*			Instance						();

			/**
			* @return Returns a new input manager. Should be destroyd with DestroyInputManager function.
			*/
			static InputManager*			CreateInputManager				();

			/**
			* @return Destroys the default input manager.
			*/
			static void						DestroyInputManager				();

			/**
			* @return Destroys a input manager.
			*/
			static void						DestroyInputManager				(InputManager* inputSystem);
		
			/**
			* Creates a device
			* @param inputType Which type of device to register
			* @param targetApplication The target proccess that will proc input.
			* @see InputDescription
			* @return Returns a handle to a device that can be rethrown to a specific device.
			*/
			virtual InputObject*			CreateDevice					( const Enum::SAIType inputType, Typedefs::WindowHandle targetApplication )	= 0;
			virtual Keyboard*				CreateKeyboardDevice			( Typedefs::WindowHandle targetApplication ) { return (Keyboard*)CreateDevice(Enum::SAIType_Keyboard, targetApplication); }
			virtual Mouse*					CreateMouseDevice				( Typedefs::WindowHandle targetApplication ) { return (Mouse*)CreateDevice(Enum::SAIType_Mouse, targetApplication); }

			/**	Enables or Disables the Input proccessing.
			*	@param The toggler.
			*/
			virtual void					ToggleInputSystem				(bool enable)												= 0;

	protected:
		InputManager();
		InputManager(const InputManager&) {}
		const InputManager& operator=(const InputManager&) {return *this;}
		virtual~InputManager();

		virtual void Destroy() = 0;
	};
}

#endif


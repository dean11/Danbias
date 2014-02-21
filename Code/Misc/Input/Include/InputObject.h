/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_INPUTOBJECT_H
#define INPUT_INPUTOBJECT_H

#include "Common.h"

namespace Input
{
	class InputObject
	{
	public:
		inline Enum::SAIType Type() { return type; }

		virtual void Activate () = 0;
		virtual void Deactivate () = 0;
		virtual bool IsActive() = 0;
		virtual bool IsInputOptionSuported(Enum::InputOptionType options) const = 0;
		virtual void SetInputOptionType(Enum::InputOptionType options) = 0;
	
	protected:
		InputObject(Enum::SAIType type) { this->type = type; }
		virtual~InputObject() {  }
	
	private:
		Enum::SAIType type;
	};
}

#endif // !INPUT_INPUTOBJECT_H

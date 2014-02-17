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

		virtual inline void Enable () { isEnabled = true; };
		virtual inline void Disable () { isEnabled = false;  };
	
	protected:
		InputObject(Enum::SAIType type) { this->type = type; }
		virtual~InputObject() {  }
	
	private:
		Enum::SAIType type;

	protected:
		bool isEnabled;
	};
}

#endif // !INPUT_INPUTOBJECT_H

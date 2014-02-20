/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_COMMON_H
#define INPUT_COMMON_H

#include "PreReq.h"
#include <math.h>

namespace Input
{
	class Keyboard;
	class Mouse;
	class InputManager;
	class InputObject;

	/*********************************************************************/
	namespace Enum
	{
		enum SAIType
		{
			SAIType_Keyboard,
			SAIType_Mouse,
			SAIType_futureExample1,
			SAIType_futureExample2,
			SAIType_futureExample3,
		};
		enum ButtonState
		{
			ButtonState_Press,			// When button is pressed (once)
			ButtonState_Down,			// When the button is held down
			ButtonState_Release,		// When button is released (once)
			ButtonState_Up,				// Default state, will not be proccesed as a callback!
		};
	}
	/*********************************************************************/
	namespace Struct
	{
		struct SAIPointInt2D
		{
			int x;
			int y;
			SAIPointInt2D() :x(0), y(0) { }
			SAIPointInt2D(int _x, int _y) :x(_x), y(_y) { }
			int Length() { return (abs(x) + abs(y)); }
		};
		struct SAIPointFloat2D
		{
			float x;
			float y;
			SAIPointFloat2D() :x(0.0f), y(0.0f) { }
			SAIPointFloat2D(float _x, float _y) :x(_x), y(_y) { }
			float Length() { return (fabs(x) + fabs(y)); }
		};

		struct InputData;
	}
	/*********************************************************************/
	namespace Typedefs
	{
		typedef void(*InputCallback)(const Struct::InputData& data);
		typedef void* DEVICE;
		typedef void* WindowHandle;
	}
	/*********************************************************************/
}

#endif // !INPUT_COMMON_H

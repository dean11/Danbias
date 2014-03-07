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
			ButtonState_Unknown,
		};
	}
	/*********************************************************************/
	namespace Struct
	{
		struct SAIPointFloat2D;

		struct SAIPointInt2D
		{
			int x;
			int y;
			SAIPointInt2D() :x(0), y(0) { }
			SAIPointInt2D(int _x, int _y) :x(_x), y(_y) { }
			SAIPointInt2D(int v[2]) :x(v[0]), y(v[1]) { }
			SAIPointInt2D(float _x, float _y) :x((int)_x), y((int)_y) { }
			SAIPointInt2D(float v[2]) :x((int)v[0]), y((int)v[1]) { }
			int Length() { return (abs(x) + abs(y)); }
		};

		struct SAIPointFloat2D
		{
			float x, y;
			
			SAIPointFloat2D() :x(0.0f), y(0.0f) { }
			SAIPointFloat2D(float _x, float _y) :x(_x), y(_y) { }
			SAIPointFloat2D(const SAIPointInt2D& p) :x((float)p.x), y((float)p.y) { }
			operator float*() { return &x; } 
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

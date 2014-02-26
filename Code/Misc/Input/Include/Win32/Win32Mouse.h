/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef INPUT_WIN32MOUSE_H
#define INPUT_WIN32MOUSE_H

#include "..\Mouse.h"
#include <Windows.h>

namespace Input
{
	class Win32Mouse :public Mouse
	{
	public:
		Win32Mouse(HWND target);
		~Win32Mouse();

		bool IsBtnUp(Enum::SAMI key) const override;
		bool IsBtnDown(Enum::SAMI key) const override;
		int GetWheelDelta()  const override;
		Struct::SAIPointInt2D& GetPixelPosition(Struct::SAIPointInt2D &targetMem = Struct::SAIPointInt2D())  const override;
		Struct::SAIPointFloat2D& GetNormalizedPosition(Struct::SAIPointFloat2D &targetMem = Struct::SAIPointFloat2D()) override;
		Struct::SAIPointFloat2D& GetDeltaPosition(Struct::SAIPointFloat2D& targetMem = Struct::SAIPointFloat2D()) const override;
	
		void Activate () override;
		void Deactivate () override;
		inline bool IsActive() override { return this->isActive; }

		void ProccessMouseData (RAWMOUSE mouse);
		bool Create( );
		void ToggleDefault( bool toggler );

	private:
		struct Buttons
		{
			unsigned int makeCode;
			bool isDown;
		};
		static const int MAXBUTTONS =Enum::SAMI_Unknown;
		Buttons buttons[MAXBUTTONS];
		RAWINPUTDEVICE device;
		Struct::SAIPointInt2D windowSize;
		bool isActive;
		Struct::SAIPointInt2D winCursPos;
	};
}

#endif // !INPUT_WIN32MOUSE_H

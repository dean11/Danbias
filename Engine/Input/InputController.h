#pragma once

#ifndef InputController_H
#define InputController_H

#include "../Window/Window.h"
//READ http://msdn.microsoft.com/en-us/library/windows/desktop/ms648380(v=vs.85).aspx#_win32_Confining_a_Cursor

namespace Oyster
{
	class Window;
	namespace Input
	{
		class Controller
		{
			friend class ::Oyster::Window;
		private:
			//Keybord events from Oyster::Window
			static void KeyPressed(const WPARAM &Key);
			//Keybord events from Oyster::Window
			static void KeyReleased(const WPARAM &Key);
			

			//Mouse events from Oyster::Window
			static void MouseMove(int x,int y);
			//Mouse events from Oyster::Window
			static void MouseBtnPressed(const WPARAM &Btn);
			static void MouseBtnReleased(const WPARAM &Btn);
		public:
			//Polling Functions
			static bool isKeyDown(const WPARAM &Key);
			static bool isKeyPressed(const WPARAM &Key);
			static bool isKeyReleased(const WPARAM &Key);

			static bool isMouseBtnDown(const WPARAM &Btn);
			static bool isMouseBtnPressed(const WPARAM &Btn);
			static bool isMouseBtnReleased(const WPARAM &Btn);

			static int GetX();
			static int GetY();

			static float GetAnalogX();
			static float GetAnalogY();

			static int GetDeltaX();
			static int GetDeltaY();

			//Controll Functions
			static void ResetDeltaX();
			static void ResetDeltaY();

			//off by default
			static void RestrictMouse(bool b = true);

			//sets x=0,y=0 to be the center oc the client area
			static void OrigoCenter();

			//default
			static void OrigoTopLeft();
		};
	}
}
#endif
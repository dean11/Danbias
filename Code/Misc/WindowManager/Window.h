#pragma once

#define NOMINMAX // Because I hate Microsoft now. ~Angry Dan.
// http://lolengine.net/blog/2011/3/4/fuck-you-microsoft-near-far-macros

#include <WinSock2.h>
#include <windows.h>
#include <sstream>
#include "..\Input\InputController.h"

namespace Oyster
{
	namespace Input
	{
		class Controller;
	}
	class Window
	{
		friend class ::Oyster::Input::Controller;
	private:
		Window();
		~Window();
		static void CaptureMouse(bool b = true);
		static bool shouldCapture;
		static bool isCaptured;
		static int OffsetMouseX;
		static int OffsetMouseY;
	public:
		static bool init( const LPCSTR appName, const LPCSTR className, const HINSTANCE &hInstance,const int &nCmdShow, WNDPROC wProc, bool ManageWindow=false );

		static void deFocus( );
		static bool isInFocus( );
		static bool mouseIsInWindow(POINT point );

		static HWND Handle;
		static std::stringstream Log;
		static RECT WindowRect;
		static RECT Size;
		static RECT ClientRect;
		static WNDPROC wndProc;

		static LRESULT CALLBACK InputMethod(HWND h,UINT i, WPARAM w,LPARAM l);

		static void SetMouseOffset(int x,int y);
	};
}
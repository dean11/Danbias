//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////
#ifndef WINDOWMANAGER_WINDOWSHELL_H
#define WINDOWMANAGER_WINDOWSHELL_H

#include <Windows.h>


class WindowShell
{
public:
	struct WINDOW_INIT_DESC
	{
		HWND				parent;					//!< Optional
		HINSTANCE			hInstance;				//!< Optional
		WNDPROC				windowProcCallback;		//!< Optional

		const wchar_t*		windowName;				//!< Optional
		POINT				windowSize;				//!< Optional
		POINT				windowPosition;			//!< Optional

		UINT				windowClassStyle;		//!< Optional
		UINT				windowStyle;			//!< Optional

		HICON				icon;					//!< Optional	
		HCURSOR				cursor;					//!< Optional	
		HBRUSH				background;				//!< Optional	

		WINDOW_INIT_DESC()
		{
			parent				= 0;
			hInstance			= NULL;
			windowName			= L"Window";
			windowSize.x		= 800;
			windowSize.y		= 600;
			windowPosition.x	= 0;
			windowPosition.y	= 0;
			windowProcCallback	= NULL;
			windowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			windowStyle			= WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION;
			//windowStyle			= WS_OVERLAPPEDWINDOW;
			icon				= LoadIcon(0, IDI_APPLICATION);
			cursor				= LoadCursor(NULL, IDC_ARROW);
			background			= (HBRUSH)GetStockObject(BLACK_BRUSH);
			//background			= (HBRUSH)GetStockObject(BACKGROUND_BLUE);(HBRUSH)(COLOR_WINDOW+1);
		}
	};

public:
	static HINSTANCE	GetHINSTANCE		();
	static HWND			GetHWND				();
	static HWND			GetParent			();
	static bool			CreateWin			(WINDOW_INIT_DESC&);
	static bool			CreateConsoleWindow	(bool redirectStdOut = true, const wchar_t* title = L"Debug Output");

	static unsigned int GetWidth();
	static unsigned int GetHeight();

	/** Procces window messages if avalible. If the return value was false, the window was destroyed. */
	static bool			Frame				();
};

#endif
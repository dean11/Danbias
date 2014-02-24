//////////////////////////////////////////////////////////
// Created 2013											//
// Dennis Andersen, Linda Andersson						//
//////////////////////////////////////////////////////////
#ifndef WINDOWMANAGER_WINDOWSHELL_H
#define WINDOWMANAGER_WINDOWSHELL_H

#include <Windows.h>

struct cPOINT :public POINT
{
	cPOINT() { x=(0); y=(0); }
	cPOINT(int width, int height) { x=(width); y=(height); }
};
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

		WINDOW_INIT_DESC(
							HWND				_parent = 0,
							HINSTANCE			_hInstance = 0,
							WNDPROC				_windowProcCallback = 0,
							const wchar_t*		_windowName = L"Window",
							POINT				_windowSize = cPOINT(800, 600),
							POINT				_windowPosition = cPOINT(0,0),
							UINT				_windowClassStyle = (CS_HREDRAW | CS_VREDRAW | CS_OWNDC),
							UINT				_windowStyle = (WS_POPUPWINDOW|WS_CAPTION),
							HICON				_icon = LoadIcon(0, IDI_APPLICATION),
							HCURSOR				_cursor = LoadCursor(NULL, IDC_ARROW),
							HBRUSH				_background = (HBRUSH)GetStockObject(BLACK_BRUSH)
						)
		{
			parent				= _parent;
			hInstance			= _hInstance;
			windowName			= _windowName;
			windowSize			= _windowSize;
			windowPosition		= _windowPosition;
			windowProcCallback	= _windowProcCallback;
			windowClassStyle	= _windowClassStyle;
			windowStyle			= _windowStyle;
			icon				= _icon;
			cursor				= _cursor;
			background			= _background;
		}
		WINDOW_INIT_DESC(
							HWND				_parent,
							HINSTANCE			_hInstance,
							WNDPROC				_windowProcCallback,
							const wchar_t*		_windowName,
							cPOINT				_windowSize,
							cPOINT				_windowPosition
						)
		{
			parent				= _parent;
			hInstance			= _hInstance;
			windowName			= _windowName;
			windowSize			= _windowSize;
			windowPosition		= _windowPosition;
			windowProcCallback	= _windowProcCallback;
			windowClassStyle	= (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
			windowStyle			= (WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION);
			icon				= LoadIcon(0, IDI_APPLICATION);
			cursor				= LoadCursor(NULL, IDC_ARROW);
			background			= (HBRUSH)GetStockObject(BLACK_BRUSH);
		}
		WINDOW_INIT_DESC(
							const wchar_t*		_windowName,
							cPOINT				_windowSize,
							cPOINT				_windowPosition
						)
		{
			parent				= 0;
			hInstance			= 0;
			windowName			= _windowName;
			windowSize			= _windowSize;
			windowPosition		= _windowPosition;
			windowProcCallback	= 0;
			windowClassStyle	= (CS_HREDRAW | CS_VREDRAW | CS_OWNDC);
			windowStyle			= (WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION);
			icon				= LoadIcon(0, IDI_APPLICATION);
			cursor				= LoadCursor(NULL, IDC_ARROW);
			background			= (HBRUSH)GetStockObject(BLACK_BRUSH);
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
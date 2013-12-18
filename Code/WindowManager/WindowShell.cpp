#include "WindowShell.h"
#include <vector>

// debug window include
#include <stdio.h>
#include <io.h>
#include <fcntl.h>


#pragma region Declarations

	struct _PrivateDataContainer
	{
		HINSTANCE							hIns;
		HWND								hWnd;
		HWND								parent;
		bool								consoleWindow;
		WNDPROC								callback;
		const wchar_t*						windowClassName;
		_PrivateDataContainer()
			:	hIns(0)
			,	hWnd(0)
			,	parent(0)
			,	consoleWindow(0)
		{ }
		~_PrivateDataContainer()   { if(this->consoleWindow) FreeConsole(); }

	} __windowShellData;

#pragma endregion

LRESULT CALLBACK DefaultWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
	PAINTSTRUCT ps;
		HDC hdc;

		switch (m) 
		{
			case WM_PAINT:
				hdc = BeginPaint(h, &ps);
				EndPaint(h, &ps);
			break;

			case WM_DESTROY:
				PostQuitMessage(0);
			break;

			case WM_KEYDOWN:
				switch(w)
				{
					case VK_ESCAPE:
						PostQuitMessage(0);
					break;
				}
			break;
		}

	return DefWindowProc(h, m, w, l);
}

HINSTANCE WindowShell::GetHINSTANCE()
{
	return __windowShellData.hIns;
}
HWND WindowShell::GetHWND()
{
	return __windowShellData.hWnd;
}
HWND WindowShell::GetParent()
{
	return __windowShellData.parent;
}
bool WindowShell::CreateWin(WINDOW_INIT_DESC &desc)
{
	if(__windowShellData.hWnd)		return false;
	if(!desc.windowProcCallback)	desc.windowProcCallback = DefaultWindowCallback;
	if(!desc.hInstance)				desc.hInstance = GetModuleHandle(0);
	if(desc.windowSize.x <= 0)		desc.windowSize.x = 50;
	if(desc.windowSize.y <= 0)		desc.windowSize.y = 50;

	
	__windowShellData.parent			= desc.parent;
	__windowShellData.hIns				= desc.hInstance;
	__windowShellData.windowClassName	= L"MainWindowShellClassName";

		#pragma region Register
	
	WNDCLASSEX wc;
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.hIconSm		= NULL;
	wc.style         = desc.windowClassStyle;
	wc.lpfnWndProc   = desc.windowProcCallback; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = __windowShellData.hIns;
	wc.hIcon         = desc.icon;
	wc.hCursor       = desc.cursor;
	wc.hbrBackground = desc.background;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = __windowShellData.windowClassName;

	if( !RegisterClassEx(&wc) )
	{
		MessageBox(0, L"Failed to register class", L"Initialization error", 0);
		return false;
	}
		#pragma endregion
	

		#pragma region Create window


	RECT rectW;
	int width;
	int height;
	DWORD style = desc.windowStyle;
	bool windowed = false;

	width = desc.windowSize.x + GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	height = desc.windowSize.y + GetSystemMetrics(SM_CYFIXEDFRAME)*2 + GetSystemMetrics(SM_CYCAPTION);

	rectW.left=(GetSystemMetrics(SM_CXSCREEN)-width)/2;
	rectW.top=(GetSystemMetrics(SM_CYSCREEN)-height)/2;
	rectW.right=rectW.left+width;
	rectW.bottom=rectW.top+height;
	
	
	if(__windowShellData.parent)
	{
		rectW.left		= 0;
		rectW.top		= 0;
		rectW.right		= desc.windowSize.x;
		rectW.bottom	= desc.windowSize.y;
		style			= WS_CHILD | WS_VISIBLE; 
		windowed		= true;
	}

	if(windowed)
	{
		__windowShellData.hWnd = CreateWindowEx(
									0,
									__windowShellData.windowClassName , 
									desc.windowName,
									style, 
									rectW.left, 
									rectW.top, 
									rectW.right - rectW.left, 
									rectW.bottom - rectW.top, 
									__windowShellData.parent, 
									NULL, 
									__windowShellData.hIns, 
									NULL
								  ); 
	}
	else
	{
		__windowShellData.hWnd = CreateWindowEx(	
									0,
									__windowShellData.windowClassName , 
									desc.windowName,
									style, 
									desc.windowPosition.x, 
									desc.windowPosition.y, 
									desc.windowSize.x, 
									desc.windowSize.y, 
									0, 
									0, 
									__windowShellData.hIns, 
									0
								  );
	}
	
	if( !__windowShellData.hWnd )
	{
		printf("Failed to create window handle : Code ( %ul )", GetLastError());
		//MessageBox(0, L"Failed to create window", L"Error!", 0);
		return false;
	}

		#pragma endregion


	//Show and update window
	ShowWindow(__windowShellData.hWnd, SW_SHOW);
	UpdateWindow(__windowShellData.hWnd);
	
	return true;
}
bool WindowShell::CreateConsoleWindow(bool redirectStdOut, const wchar_t* title)
{
	// allocate a console for this app
	if(AllocConsole() == FALSE)	return false;

	if(redirectStdOut)
	{
		// redirect unbuffered STDOUT to the console
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
		FILE *fp = _fdopen( fileDescriptor, "w" );
		*stdout = *fp;
		setvbuf( stdout, NULL, _IONBF, 0 );
	
	
		// give the console window a bigger buffer size
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
		{
			COORD bufferSize;
			bufferSize.X = csbi.dwSize.X;
			bufferSize.Y = 50;
			SetConsoleScreenBufferSize(consoleHandle, bufferSize);
		}
	}

	// give the console window a nicer title
	SetConsoleTitle(title);

	return true;
}
unsigned int WindowShell::GetWidth()
{
	if(!__windowShellData.hWnd) return -1;

	RECT rect;
	GetWindowRect(__windowShellData.hWnd, &rect);

	return (rect.right - rect.left);
}
unsigned int WindowShell::GetHeight()
{
	if(!__windowShellData.hWnd) return -1;

	RECT rect;
	GetWindowRect(__windowShellData.hWnd, &rect);

	return (rect.bottom - rect.top);
}
bool WindowShell::Frame()
{
	MSG msg = {0};
	while (true)
	{
		if(!__windowShellData.parent)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{ 
				if (msg.message == WM_QUIT)	return false;

				DispatchMessage(&msg);
				continue;
			}
		}

		break;
	}

	return true;
}

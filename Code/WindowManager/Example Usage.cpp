
#include "WindowShell.h"




/********************************************
 *		Callback for main window			*
 *******************************************/
LRESULT CALLBACK 
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)	
{ 
	PAINTSTRUCT ps;
	HDC hdc;

	switch( msg )
	{
		// Window is being destroyed.
		case WM_DESTROY:
			PostQuitMessage(0);
		break;

		case WM_CREATE:
		break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		break;

		// Prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100; 
		break;

		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
				PostQuitMessage(0);
		break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

	}
	return 0;
}



/********************************************
 *		Callback for children				*
 *******************************************/
LRESULT CALLBACK 
ChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)	
{ 
	PAINTSTRUCT ps;
	HDC hdc;

	switch( msg )
	{
		// Window is being destroyed.
		case WM_DESTROY:
			WindowShell::self()->removeChild(hwnd);
		break;

		case WM_CREATE:
		break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
		break;

		// Prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100; 
		break;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}



int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

/********************************************
 *			Description of a window			*
 *******************************************/
	WindowShell::WINDOW_INIT_DESC wDesc;
	//wDesc.hInstance = hInst;
	wDesc.windowPosition.x = 50;
	wDesc.windowPosition.y = 50;
	wDesc.windowSize.x = 1024;
	wDesc.windowSize.x = 800;
	wDesc.windowProcCallback = WndProc;

	WindowShell::self()->createWin(wDesc);



/********************************************
 *		The main application loop			*
 *******************************************/
	MSG msg = {0};
	while(msg.message != WM_QUIT)
	{
		// If there are Window messages process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		else
        {	

        }
    }


/************************************************
 *		Destroying the WindowShell singleton	*
 ***********************************************/
	WindowShell::destroy();

	return cmdShow;
}

#include "Window.h"
#include <WindowsX.h>

using namespace Oyster;

HWND Window::Handle = NULL;
std::stringstream Window::Log = std::stringstream();
RECT Window::WindowRect = RECT();
RECT Window::Size = RECT();
RECT Window::ClientRect = RECT();
WNDPROC Window::wndProc = NULL;
bool Window::isCaptured = false;
bool Window::shouldCapture = false;
int Window::OffsetMouseX=0;
int Window::OffsetMouseY=0;

bool Window::init( const LPCSTR appName, const LPCSTR className, const HINSTANCE &hInstance, const int &nCmdShow, WNDPROC wProc, bool handleLoop )
{
	if( Handle != NULL )
	{
		Log<< "Faulty Init, First window Exists";
		return true;
	}

		// Register window class
		WNDCLASSEXA window	  = { 0 };
		window.cbSize		  = sizeof( WNDCLASSEXA ); 
		window.style          = CS_HREDRAW | CS_VREDRAW;
		window.lpfnWndProc    = wProc;
		if(handleLoop)
		{
			window.lpfnWndProc = Window::InputMethod;
			Window::wndProc = wProc;
		}
		window.hInstance      = hInstance;
		window.hIcon          = LoadIcon( NULL, IDI_APPLICATION );
		window.hCursor        = LoadCursor( NULL, IDC_ARROW );
		window.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
		window.lpszMenuName   = NULL;
		window.lpszClassName  = className;
	
		if( !RegisterClassExA( &window ) )
		{
			Log << "Failed to Register Window.\n";
			return false;
		}
 
		Handle = CreateWindowEx( NULL,
								className,
								appName,
								WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT,
								CW_USEDEFAULT,
								800, //rc.right - rc.left,
								600, //rc.bottom - rc.top,
								NULL,
								NULL,
								hInstance,
								NULL );

		if( Handle == NULL )
		{
			Log << "Failed to Init Window.\n";
			return false;
		}
		WINDOWINFO wi;
		wi.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(Handle,&wi);
		WindowRect = wi.rcClient;
		Size.right = Size.left = WindowRect.right - WindowRect.left;
		Size.top = Size.bottom = WindowRect.bottom - WindowRect.top;
		ClientRect.left= ClientRect.top=0;
		ClientRect.right = Size.left;
		ClientRect.bottom = Size.top;
		ShowWindow( Handle, nCmdShow );
		return true;
}

void Window::deFocus( ) { SetFocus( GetDesktopWindow() ); }
bool Window::isInFocus( ) { return Handle == GetFocus( ); }
bool Window::mouseIsInWindow(POINT point )
{
	return PtInRect( &WindowRect, point )!=0; 
}

LRESULT CALLBACK Window::InputMethod( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{

	case WM_KEYDOWN:
		Oyster::Input::Controller::KeyPressed(wParam);
		break;
	case WM_KEYUP:
		Oyster::Input::Controller::KeyReleased(wParam);
		break;
	case WM_MOUSEMOVE:
		if(!isCaptured)
			if(shouldCapture)
				SetCapture(Handle);
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);
		if(PtInRect(&ClientRect,pt)!=0)
			Oyster::Input::Controller::MouseMove(GET_X_LPARAM(lParam)+OffsetMouseX,GET_Y_LPARAM(lParam)+OffsetMouseY);
		else
		{
			if(shouldCapture)
			{
				pt.x = Input::Controller::GetX();
				pt.y = Input::Controller::GetY();
				ClientToScreen(Handle, &pt);
				SetCursorPos(pt.x-OffsetMouseX,pt.y-OffsetMouseY);
			}
		}
	break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDOWN:
		{
			Oyster::Input::Controller::MouseBtnPressed(wParam);
			break;
		}
	case WM_LBUTTONUP:
		{
			Oyster::Input::Controller::MouseBtnReleased(MK_LBUTTON);
			break;
		}
	case WM_MBUTTONUP:
		{
			Oyster::Input::Controller::MouseBtnReleased(MK_MBUTTON);
			break;
		}
	case WM_RBUTTONUP:
		{
			Oyster::Input::Controller::MouseBtnReleased(MK_RBUTTON);
			break;
		}
	case WM_XBUTTONUP:
		{
			Oyster::Input::Controller::MouseBtnReleased(wParam);
			break;
		}
	default:
		return wndProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Window::CaptureMouse(bool b)
{
	shouldCapture=b;
	isCaptured=false;
	if(!b)
		SetCapture(NULL);
}

void Window::SetMouseOffset(int x,int y)
{
	OffsetMouseX=x;
	OffsetMouseY=y;
}
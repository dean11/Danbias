#include <Windows.h>

const unsigned short raw_input_usage_keyboard = 6;
const unsigned short raw_input_usage_mouse = 2;
HWND winHandle;
bool isPointAndClickMode;
float normalizedMousePosX, normalizedMousePosY;

LRESULT CALLBACK RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l);
LRESULT RawInputParser(HWND h, LPARAM l);
void Update( );

//int main()
int WINAPI WinMain( HINSTANCE hinst, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	WNDCLASSEXW wc;
	wc.cbSize		 = sizeof(WNDCLASSEXW);
	wc.hIconSm		 = NULL;
	wc.style         = NULL;
	wc.lpfnWndProc   = RawWindowCallback; 
	wc.cbClsExtra    = NULL;
	wc.cbWndExtra    = NULL;
	wc.hInstance     = hinst;
	wc.hIcon         = NULL;
	wc.hCursor       = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = L"RawInputTest";

	if( !RegisterClassExW(&wc) )
	{
		const char *breakpoint = "";
	}

	winHandle = CreateWindowExW( 0, L"RawInputTest", L"RawInputTest", WS_OVERLAPPEDWINDOW | WS_CAPTION, 0, 0, 600, 400, NULL, NULL, hinst, NULL );
	ShowWindow( winHandle, cmdShow );

	if( !winHandle )
	{
		const char *breakpoint = "";
	}

	isPointAndClickMode = true;
	normalizedMousePosX =
	normalizedMousePosY = 0.5f;

	MSG msg = {0};
	while( true )
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{ 
			if (msg.message == WM_QUIT)	break;
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}
	return 0;
}

LRESULT CALLBACK RawWindowCallback(HWND h, UINT m, WPARAM w, LPARAM l)
{
	LRESULT val = 0;
	switch (m)
	{
		case WM_INPUT:
			{
				RawInputParser( h, l );
			}
			break;
		case WM_KEYUP:
			{
				if( w == 16 )
				{
					const char *breakpoint = "";

					RAWINPUTDEVICE rid;
					rid.usUsagePage = 0x01;
					rid.hwndTarget = winHandle;
					rid.usUsage = raw_input_usage_keyboard;
					rid.dwFlags = RIDEV_NOLEGACY;
					if( RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == FALSE )
					{
						const char *breakpoint = "";
					}
				}
			}
			break;	
		case WM_RBUTTONUP:
			{
				const char *breakpoint = "";
	
				RAWINPUTDEVICE rid;
				rid.usUsagePage = 0x01;
				rid.hwndTarget = winHandle;
				rid.usUsage = raw_input_usage_mouse;
				rid.dwFlags = RIDEV_NOLEGACY | RIDEV_CAPTUREMOUSE;
				if( RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == TRUE )
				{
					ShowCursor( false );
					isPointAndClickMode = false;
				}
				else
				{
					const char *breakpoint = "";
				}				
			}
			break;
		case WM_ACTIVATE:
			{
				const char *breakpoint = "";
			}
			break;
		break;
		case WM_CREATE:
			{
				const char *breakpoint = "";
			}
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
		default: break;
	}

	return DefWindowProc(h, m, w, l);
}

LRESULT RawInputParser(HWND h, LPARAM l)
{
	//Get The size of the raw data buffer
	UINT bufferSize;
	GetRawInputData((HRAWINPUT)l, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));
	if (bufferSize < 1) 
	{ return 0; } 

	//Create and read the raw input data
	LPBYTE rawBuffer = new BYTE[bufferSize];
	UINT readBytes = GetRawInputData((HRAWINPUT)l, RID_INPUT, rawBuffer, &bufferSize,  sizeof(RAWINPUTHEADER));
	if ( readBytes != bufferSize )
	{
		delete [] rawBuffer;
		return 0;
	}

	HRESULT result = 0;
	RAWINPUT* raw = (RAWINPUT*)rawBuffer;

	if( raw->header.dwType == RIM_TYPEKEYBOARD )	
	{	
		if( (raw->data.keyboard.Flags & RI_KEY_BREAK) && raw->data.keyboard.VKey == 16 )
		{
			const char *breakpoint = "";

			RAWINPUTDEVICE rid;
			rid.usUsagePage = 0x01;
			rid.hwndTarget = NULL;
			rid.usUsage = raw_input_usage_keyboard;
			rid.dwFlags = RIDEV_REMOVE;
			if( RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == FALSE )
			{
				const char *breakpoint = "";
			}
		}
	}
	else if( raw->header.dwType == RIM_TYPEMOUSE )
	{
		if( raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP )
		{
			const char *breakpoint = "";

			RAWINPUTDEVICE rid;
			rid.usUsagePage = 0x01;
			rid.hwndTarget = NULL;
			rid.usUsage = raw_input_usage_mouse;
			rid.dwFlags = RIDEV_REMOVE;
			if( RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)) == TRUE )
			{
				RECT winRect;
				GetWindowRect( winHandle, &winRect );
				SetCursorPos( (winRect.left + winRect.right) >> 1, (winRect.top + winRect.bottom) >> 1 );
				isPointAndClickMode = true;
			}
			else
			{
				const char *breakpoint = "";
			}
		}
	}
	else result = DefRawInputProc(&raw, 1, sizeof(RAWINPUTHEADER));	

	delete [] rawBuffer;
	return result;
}

void Update( )
{
	if( isPointAndClickMode )
	{
		{ // Calculating and storing the normalizedMousePos values
			POINT clientReach, clientCenter, mousePos;

			RECT winRect, clientRect;
			GetClientRect( winHandle, &clientRect );
			GetWindowRect( winHandle, &winRect );

			LONG borderThickness = (winRect.right - winRect.left - clientRect.right) >> 1;

			clientReach.x = clientRect.right >> 1;
			clientReach.y = clientRect.bottom >> 1;

			clientCenter.x = (winRect.left + winRect.right) >> 1;
			clientCenter.y = winRect.bottom - clientReach.y - borderThickness;

			GetCursorPos( &mousePos );

			normalizedMousePosX = ((float)(mousePos.x - clientCenter.x + clientReach.x)) / (float)clientRect.right;
			normalizedMousePosY = ((float)(mousePos.y - clientCenter.y + clientReach.y)) / (float)clientRect.bottom;
		}

		// Check if normalizedMousePos intersects client surface [(0,0), (1,1)]
		bool mouseIsNowWithinBounds = true;
		if     ( normalizedMousePosX < 0.0f ) mouseIsNowWithinBounds = false;
		else if( normalizedMousePosX > 1.0f ) mouseIsNowWithinBounds = false;
		else if( normalizedMousePosY < 0.0f ) mouseIsNowWithinBounds = false;
		else if( normalizedMousePosY > 1.0f ) mouseIsNowWithinBounds = false;

		// Detect onEnter or onExit case
		static bool mouseWereWithinBounds = false;
		if( mouseIsNowWithinBounds & !mouseWereWithinBounds )
		{ // onEnter
			ShowCursor( false );
			mouseWereWithinBounds = mouseIsNowWithinBounds;
		}
		else if( !mouseIsNowWithinBounds & mouseWereWithinBounds )
		{ // onExit
			ShowCursor( true );
			mouseWereWithinBounds = mouseIsNowWithinBounds;
		}
	}
}
#include "RawInputWrapper.h"
// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <vld.h>
#include <cassert>


HWND hWnd;
// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
        {
            // close the application entirely
            PostQuitMessage(0);
            return 0;
        } break;
		case WM_KEYUP:
			MessageBox(0, L"WM_KEYUP", L"", 0);
		break;
		case WM_KEYDOWN:
			MessageBox(0, L"WM_KEYDOWN", L"", 0);
		break;
		//case WM_INPUT:
		//	MessageBox(0, L"WM_INPUT_MAIN", L"", 0);
		//break;
    }
	
    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}
void initWindow(HINSTANCE h, int i)
{
	// this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = h;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = L"WindowClass1";

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(NULL,
                          L"WindowClass1",					// name of the window class
                          L"Our First Windowed Program",	// title of the window
                          WS_OVERLAPPEDWINDOW,				// window style
                          300,								// x-position of the window
                          300,								// y-position of the window
                          500,								// width of the window
                          400,								// height of the window
                          NULL,								// we have no parent window, NULL
                          NULL,								// we aren't using menus, NULL
                          h,						// application handle
                          NULL);							// used with multiple windows, NULL

    // display the window on the screen
    ShowWindow(hWnd, i);
}
void initRaw()
{
	RawInputWrapper::Self()->Input_AddDevice(hWnd);
}
// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initWindow(hInstance, nCmdShow);
	initRaw();


    // this struct holds Windows event messages
    MSG msg;

    // wait for the next message in the queue, store the result in 'msg'
    while(GetMessage(&msg, NULL, 0, 0))
    {
        // translate keystroke messages into the right format
        TranslateMessage(&msg);
        // send the message to the WindowProc function
        DispatchMessage(&msg);

		RawInputWrapper::Self()->Input_Read();
    }

	RawInputWrapper::Destroy();

    return msg.wParam;
}
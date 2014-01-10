#define NOMINMAX
#include "WinTimer.h"
#include "WindowShell.h"
#include "GFXAPI.h"
#include "PhysicsAPI.h"

using namespace ::Utility;
using namespace ::Oyster;
using namespace ::Oyster::Math3D;

// typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MessagePipe( HWND, UINT, WPARAM, LPARAM );

int WINAPI WinMain( HINSTANCE thisInstance, HINSTANCE prevInst, PSTR cmdLine, int cmdShow)
{
	if( SetDllDirectoryW(L"..\\DLL") == FALSE )
	{
		return 0;
	}

	WinTimer timer;
	WindowShell screen;
	{
		WindowShell::WINDOW_INIT_DESC desc;
		desc.parent				= nullptr;
		desc.hInstance			= thisInstance;
		desc.windowProcCallback	= MessagePipe;
		desc.windowName			= L"Physics Testing Enviroment";
		desc.windowSize.x		= 1280;
		desc.windowSize.y		= 720;
		desc.windowPosition.x	= 0;
		desc.windowPosition.y	= 0;
		desc.windowClassStyle	= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		desc.windowStyle		= WS_POPUPWINDOW|WS_SYSMENU|WS_CAPTION;
		desc.icon				= LoadIcon(0, IDI_APPLICATION);
		desc.cursor				= LoadCursor(NULL, IDC_ARROW);
		desc.background			= (HBRUSH)GetStockObject(BLACK_BRUSH);

		if( !screen.CreateWin(desc) )
		{
			return 0;
		}
	}
	
	if( Graphics::API::Init(screen.GetHWND(), false, false, Float2(1280.0f, 720.0f)) == Graphics::API::Fail )
	{
		return 0;		
	}

	const Float updatePeriodicy = 1.0f / 120.0f;
	Float accumulatedTime = 0.0f;
	Physics::API::Instance().Init( 27, 1, Float3(1024.0f) );
	Physics::API::Instance().SetFrameTimeLength( updatePeriodicy );
	
	//// INIT STUFF

	//// END INITSTUFF

	timer.reset();
	while( true )
	{
		if( screen.Frame() )
		{
			accumulatedTime += (Float)timer.getElapsedSeconds();
			timer.reset();

			while( accumulatedTime >= updatePeriodicy )
			{
				Physics::API::Instance().Update();
				accumulatedTime -= updatePeriodicy;
			}
		}
		else break;
	}

	return 0;
}

LRESULT CALLBACK MessagePipe( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message) 
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
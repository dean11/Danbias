//--------------------------------------------------------------------------------------
// File: TemplateMain.cpp
//
// BTH-D3D-Template
//
// Copyright (c) Stefan Petersson 2011. All rights reserved.
//--------------------------------------------------------------------------------------
#define NOMINMAX
#include <Windows.h>

#include "DllInterfaces/GFXAPI.h"
#include "IGame.h"

#include "L_inputClass.h"

// debug window include
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>




//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;

GameLogic::IGame* game;
InputClass* inputObj;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT             InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT				Render(float deltaTime);
HRESULT				Update(float deltaTime);
HRESULT				InitGame();
HRESULT				CleanUp();



//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------

void SetStdOutToNewConsole()
{
	// allocate a console for this app
	AllocConsole();

	// redirect unbuffered STDOUT to the console
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
	FILE *fp = _fdopen( fileDescriptor, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// give the console window a nicer title

	SetConsoleTitle(L"Debug Output");

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

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	BOOL success = SetDllDirectory(L"..\\..\\DLL");
	if (success == 0)
	{
		return 0;
	}


	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitGame() ) )
		return 0;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
	
	//debug window
	//SetStdOutToNewConsole();

	// Main message loop
	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			//render
			Update(dt);
			Render(dt);

			prevTimeStamp = currTimeStamp;
		}
	}
	CleanUp();
	return (int) msg.wParam;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = L"BTH_D3D_Template";
	wcex.hIconSm        = 0;
	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	// Adjust and create window
	g_hInst = hInstance; 
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(g_hWnd = CreateWindow(
		L"BTH_D3D_Template",
		L"BTH - Direct3D 11.0 Template",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL)))
	{
		return E_FAIL;
	}

	ShowWindow( g_hWnd, nCmdShow );

	return S_OK;
}


HRESULT InitGame()
{
	inputObj = new InputClass;
	if(!inputObj->Initialize(g_hInst, g_hWnd, 1024, 768))
	{
		MessageBox(0, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}
	if(Oyster::Graphics::API::Init(g_hWnd, false, false, Oyster::Math::Float2( 1024, 768)) != Oyster::Graphics::API::Sucsess)
		return E_FAIL;


	game = new GameLogic::IGame();
	game->Init();
	game->StartGame();



	return S_OK;
}
HRESULT Update(float deltaTime)
{
	inputObj->Update();
	GameLogic::keyInput key = GameLogic::keyInput_none;

	if(inputObj->IsKeyPressed(DIK_W))
	{
		key = GameLogic::keyInput_W;	
	}
	else if(inputObj->IsKeyPressed(DIK_A))
	{
		key = GameLogic::keyInput_A;	
	}
	else if(inputObj->IsKeyPressed(DIK_S))
	{
		key = GameLogic::keyInput_S;	
	}
	else if(inputObj->IsKeyPressed(DIK_D))
	{
		key = GameLogic::keyInput_D;	
	}

	game->Update(key);
	
	return S_OK;
}

HRESULT Render(float deltaTime)
{
	int isPressed = 0;
	if(inputObj->IsKeyPressed(DIK_A))
	{
		isPressed = 1;
		//std::cout<<"test";
	}

	// test view and projection matrix 
	Oyster::Math::Float3 dir = Oyster::Math::Float3(0,0,-1);
	Oyster::Math::Float3 up  =Oyster::Math::Float3(0,1,0);
	Oyster::Math::Float3 pos = Oyster::Math::Float3(0, 0, 100);

	Oyster::Math::Float4x4 view  =Oyster::Math3D::OrientationMatrix_LookAtDirection(dir, up, pos);
	view = view.GetInverse();

	Oyster::Math::Float4x4 proj = Oyster::Math3D::ProjectionMatrix_Perspective(3.14f/2, 1024/768, 1, 1000);

	Oyster::Graphics::API::NewFrame(view, proj);
	
	game->Render();
	wchar_t title[255];
	swprintf(title, sizeof(title), L"| Pressing A:  %d | \n", (int)(isPressed));
	SetWindowText(g_hWnd, title);

	Oyster::Graphics::API::EndFrame();

	return S_OK;
}

HRESULT CleanUp()
{
	
	if(game)
	{
		delete game;
		game = NULL;
	}
	return S_OK;
}
//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:

		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


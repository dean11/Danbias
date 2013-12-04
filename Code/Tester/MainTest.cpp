//--------------------------------------------------------------------------------------
// File: TemplateMain.cpp
//
// BTH-D3D-Template
//
// Copyright (c) Stefan Petersson 2011. All rights reserved.
//--------------------------------------------------------------------------------------
#define NOMINMAX
#include <vld.h>
#include <Windows.h>
#include "DllInterfaces\GFXAPI.h"



//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;
Oyster::Graphics::Model::Model* m				= NULL;
Oyster::Graphics::Model::Model* m2				= NULL;
Oyster::Math::Float4x4 V;
Oyster::Math::Float4x4 P;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT             InitWindow( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT				Render(float deltaTime);
HRESULT				Update(float deltaTime);
HRESULT				InitDirect3D();




//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{

	BOOL b = SetDllDirectoryW(L"..\\DLL");
	typedef struct tagLOADPARMS32
	{ 
		LPSTR lpEnvAddress;  // address of environment strings 
		LPSTR lpCmdLine;     // address of command line 
		LPSTR lpCmdShow;     // how to show new program 
		DWORD dwReserved;    // must be zero 
	} LOADPARMS32;
	LOADPARMS32 params;
	params.dwReserved=NULL;
	params.lpCmdLine="";
	params.lpCmdShow="";
	params.lpEnvAddress="";

	if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
		return 0;

	if( FAILED( InitDirect3D() ) )
		return 0;

	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

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

	Oyster::Graphics::API::DeleteModel(m);
	Oyster::Graphics::API::Clean();
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



//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDirect3D()
{
	HRESULT hr = S_OK;;

	if(Oyster::Graphics::API::Init(g_hWnd,false,false, Oyster::Math::Float2( 1024, 768 )) == Oyster::Graphics::API::Fail)
	{
		return E_FAIL;
	}

#pragma region Triangle
	//Oyster::Graphics::Definitions::ObjVertex mesh[] =
	//{
	//	{Oyster::Math::Vector3(-1,1,0),Oyster::Math::Vector2(0,0),Oyster::Math::Vector3(1,1,0)},
	//	{Oyster::Math::Vector3(1,-1,0),Oyster::Math::Vector2(0,0),Oyster::Math::Vector3(1,1,0)},
	//	{Oyster::Math::Vector3(1,1,0),Oyster::Math::Vector2(0,0),Oyster::Math::Vector3(1,1,0)},
	//};

	//Oyster::Graphics::Buffer::BUFFER_INIT_DESC desc;
	//desc.ElementSize= sizeof(Oyster::Graphics::Definitions::ObjVertex);
	//desc.NumElements = 3;
	//desc.InitData=mesh;
	//desc.Type = Oyster::Graphics::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
	//desc.Usage = Oyster::Graphics::Buffer::BUFFER_USAGE::BUFFER_USAGE_IMMUTABLE;

	//Oyster::Graphics::Buffer *b = new Oyster::Graphics::Buffer();;
	//b->Init(desc);

	////b.Apply(0);
	//Oyster::Graphics::Render::ModelInfo* mi = new Oyster::Graphics::Render::ModelInfo();
	//mi->Indexed = false;
	//mi->VertexCount = 3;
	//mi->Vertices = b;
	
	//m->info = mi;
#pragma endregion
	
#pragma region Obj
	m =  Oyster::Graphics::API::CreateModel(L"crate");
	m2 = Oyster::Graphics::API::CreateModel(L"crate");
	m2->WorldMatrix = Oyster::Math3D::OrientationMatrix(Oyster::Math::Float3::null,Oyster::Math::Float3(0,5,0),Oyster::Math::Float3::null);
#pragma endregion
	

	P = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,1024.0f/768.0f,.1f,1000);

	V = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),Oyster::Math::Float3(0,0,5.4f));
	V = Oyster::Math3D::InverseOrientationMatrix(V);


	return S_OK;
}
float angle = 0;
HRESULT Update(float deltaTime)
{
	angle += Oyster::Math::pi/30000;
	m->WorldMatrix =  Oyster::Math3D::RotationMatrix_AxisY(angle);
	m2->WorldMatrix = Oyster::Math3D::OrientationMatrix(Oyster::Math::Float3(0,0,1)*-angle,Oyster::Math::Float3(0,4,0),Oyster::Math::Float3::null);
	return S_OK;
}

HRESULT Render(float deltaTime)
{
	Oyster::Graphics::API::NewFrame(V,P);

	Oyster::Graphics::API::RenderScene(m,1);
	Oyster::Graphics::API::RenderScene(m2,1);

	Oyster::Graphics::API::EndFrame();

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


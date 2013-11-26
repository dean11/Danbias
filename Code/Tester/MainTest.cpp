//--------------------------------------------------------------------------------------
// File: TemplateMain.cpp
//
// BTH-D3D-Template
//
// Copyright (c) Stefan Petersson 2011. All rights reserved.
//--------------------------------------------------------------------------------------
#define NOMINMAX
#include <Windows.h>
#include "Core/Core.h"
#include "Render\Preparations\Preparations.h"
#include "Render\Resources\Resources.h"
#include "Render\Rendering\Render.h"
#include "FileLoader\ObjReader.h"
#include "Definitions\GraphicalDefinition.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;
Oyster::Graphics::Model::Model* m	=				new Oyster::Graphics::Model::Model();
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

	Oyster::Graphics::Core::resolution = Oyster::Math::Float2( 1024, 768 );

	if(Oyster::Graphics::Core::Init::FullInit(g_hWnd,false,false)==Oyster::Graphics::Core::Init::Fail)
		return E_FAIL;

	//Init shaders
	Oyster::Graphics::Render::Resources::Init();

	Oyster::Graphics::Render::Preparations::Basic::SetViewPort();

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
	OBJReader or;
	or.readOBJFile(L"crate.obj");
	m->info = (void*)or.toModel();
	m->Visible=true;
#pragma endregion
	
	m->WorldMatrix = Oyster::Math::Matrix::identity;

	P = Oyster::Math3D::ProjectionMatrix_Perspective(Oyster::Math::pi/2,16.0f/9.0f,.1f,100);

	V = Oyster::Math3D::OrientationMatrix_LookAtDirection(Oyster::Math::Float3(0,0,-1),Oyster::Math::Float3(0,1,0),Oyster::Math::Float3(0,-1.5f,10.4f));
	V = Oyster::Math3D::InverseOrientationMatrix(V);


	return S_OK;
}

HRESULT Update(float deltaTime)
{
	return S_OK;
}

HRESULT Render(float deltaTime)
{
	Oyster::Graphics::Render::Rendering::Basic::NewFrame(V,P);
	//Oyster::Graphics::Render::Preparations::Basic::ClearBackBuffer(Oyster::Math::Float4(0,0,1,1));

	//m->info->Vertices->Apply(0);

	//Oyster::Graphics::Core::deviceContext->Draw(3,0);

	//Oyster::Graphics::Core::swapChain->Present(0,0);

	Oyster::Graphics::Render::Rendering::Basic::RenderScene(m,1);

	Oyster::Graphics::Render::Rendering::Basic::EndFrame();

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


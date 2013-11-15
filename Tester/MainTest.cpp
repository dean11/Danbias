//--------------------------------------------------------------------------------------
// File: TemplateMain.cpp
//
// BTH-D3D-Template
//
// Copyright (c) Stefan Petersson 2011. All rights reserved.
//--------------------------------------------------------------------------------------
#define NOMINMAX
#include <Windows.h>
#include "Engine.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE				g_hInst					= NULL;  
HWND					g_hWnd					= NULL;



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

	Oyster::Engine::Init::Setup setup;
	setup.Fullscreen = false;
	setup.ForceDX11 = true;
	setup.SingleThreaded = true;
	setup.window = g_hWnd;

	Oyster::Engine::Init::FullInit( setup );

	std::wstring ShaderPath = L"..\\OysterGraphics\\Shader\\HLSL\\";
	std::wstring EffectPath = L"SimpleDebug\\";

	Oyster::Core::ShaderManager::Init(ShaderPath + EffectPath + L"DebugPixel.hlsl",Oyster::Core::ShaderManager::ShaderType::Pixel,L"Debug",false);
	Oyster::Core::ShaderManager::Init(ShaderPath + EffectPath + L"DebugVertex.hlsl",Oyster::Core::ShaderManager::ShaderType::Vertex,L"Debug",false);

	Oyster::Core::ShaderManager::Set::Vertex(Oyster::Core::ShaderManager::Get::Vertex(L"Debug"));
	Oyster::Core::ShaderManager::Set::Pixel(Oyster::Core::ShaderManager::Get::Pixel(L"Debug"));

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* layout;

	Oyster::Core::ShaderManager::CreateInputLayout( inputDesc, 1, Oyster::Core::ShaderManager::Get::Vertex(L"Debug"), layout);

	Oyster::Core::DeviceContext->IASetInputLayout(layout);
	Oyster::Core::DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Oyster::Engine::PrepareForRendering::BindBackBuffer();
	
	struct float4
	{
		float x,y,z,w;
	};

	float4 mesh[] =
	{
		{-1.0f,1.0f,0.0f,1.0f},
		{1.0f,1.0f,0.0f,1.0f},
		{1.0f,-1.0f,0.0f,1.0f},
	};

	Oyster::Buffer::BUFFER_INIT_DESC desc;
	desc.ElementSize= sizeof(float4);
	desc.NumElements = 3;
	desc.InitData=mesh;
	desc.Type = Oyster::Buffer::BUFFER_TYPE::VERTEX_BUFFER;
	desc.Usage = Oyster::Buffer::BUFFER_USAGE::BUFFER_USAGE_IMMUTABLE;

	Oyster::Buffer b;
	b.Init(desc);
	b.Apply(0);

	return S_OK;
}

HRESULT Update(float deltaTime)
{
	return S_OK;
}

HRESULT Render(float deltaTime)
{
	Oyster::Engine::PrepareForRendering::ClearBackBuffer(Oyster::Math::Float4(0,0,1,1));

	Oyster::Core::DeviceContext->Draw(3,0);

	Oyster::Core::SwapChain->Present(0,0);

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

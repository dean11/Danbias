#define NOMINMAX
#include <Windows.h>
#include "Include\DanBiasGame.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameClientState/GameClientState.h"
#include "GameClientState\GameState.h"
#include "GameClientState\LobbyState.h"
#include "PlayerProtocols.h"
#include "NetworkClient.h"

#include "L_inputClass.h"
#include "vld.h"

namespace DanBias
{
	__int64 DanBiasGame::cntsPerSec		= 0;
	__int64 DanBiasGame::prevTimeStamp	= 0;
	float DanBiasGame::secsPerCnt		= 0;
	HINSTANCE DanBiasGame::g_hInst		= NULL;
	HWND DanBiasGame::g_hWnd			= NULL;

#pragma region Game Data


	struct MyRecieverObject :public Oyster::Network::ProtocolRecieverObject
	{
	Oyster::Network::NetworkClient* nwClient;
	Client::GameClientState* gameClientState;
	
	void ProtocolRecievedCallback(Oyster::Network::CustomNetProtocol& p) override
	{

		int pType = p[0].value.netInt;
		//Client::GameClientState::ProtocolStruct* protocolData; 
		switch (pType)
		{
		case protocol_Gamplay_PlayerNavigation:
			{

		
				Client::GameClientState::KeyInput* protocolData = new Client::GameClientState::KeyInput;
				for(int i = 0; i< 6; i++)
				{
					protocolData->key[i] = p[i+1].value.netBool;
				}

				((Client::GameState*)gameClientState)->Protocol(protocolData);
				delete protocolData;
				protocolData = NULL;
			}
			break;
		case protocol_Gamplay_PlayerPosition:
			{
				Client::GameClientState::PlayerPos* protocolData = new Client::GameClientState::PlayerPos;
				for(int i = 0; i< 3; i++)
				{
					protocolData->playerPos[i] = p[i].value.netFloat;
				}
				//if(dynamic_cast<Client::GameState*>(gameClientState))
				gameClientState->Protocol(protocolData);
				delete protocolData;
				protocolData = NULL;
			}
			break;


		case protocol_Gamplay_ObjectPosition:
			{

				Client::GameClientState::ObjPos* protocolData = new Client::GameClientState::ObjPos;
				protocolData->object_ID = p[1].value.netInt;
				for(int i = 0; i< 16; i++)
				{
					protocolData->worldPos[i] = p[i+2].value.netFloat;
				}

		
				gameClientState->Protocol(protocolData);
			
				delete protocolData;
				protocolData = NULL;
			}
			break;

		default:
			break;
		}	
	}
	};
	class DanBiasGamePrivateData
	{

	public:
		DanBiasGamePrivateData()
		{

		}
		~DanBiasGamePrivateData()
		{

		}

	public:
		//Client::GameClientState* gameClientState;
		InputClass* inputObj;
		MyRecieverObject* recieverObj;

	} data;
#pragma endregion


	DanBiasGamePrivateData* DanBiasGame::m_data = new DanBiasGamePrivateData();

	//--------------------------------------------------------------------------------------
	// Interface API functions
	//--------------------------------------------------------------------------------------
	DanBiasClientReturn DanBiasGame::Initiate(DanBiasGameDesc& desc)
	{
		if( FAILED( InitWindow( desc.hinst, desc.nCmdShow ) ))
			return DanBiasClientReturn_Error;

		if( FAILED( InitDirect3D() ) )
			return DanBiasClientReturn_Error;

		if( FAILED( InitInput() ) )
			return DanBiasClientReturn_Error;

		cntsPerSec = 0;
		QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
		secsPerCnt = 1.0f / (float)cntsPerSec;

		prevTimeStamp = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

		m_data->recieverObj = new MyRecieverObject;
		
		m_data->recieverObj->nwClient = new Oyster::Network::NetworkClient(m_data->recieverObj, Oyster::Network::NetworkProtocolCallbackType_Object);
		m_data->recieverObj->nwClient->Connect(desc.port, desc.IP);

		if (!m_data->recieverObj->nwClient->IsConnected())
		{
			// failed to connect
			return DanBiasClientReturn_Error;
		}
		// Start in lobby state
		m_data->recieverObj->gameClientState = new  Client::LobbyState();
		m_data->recieverObj->gameClientState->Init(m_data->recieverObj->nwClient);


		return DanBiasClientReturn_Sucess;
	}

	DanBiasClientReturn DanBiasGame::Run()
	{
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
				if(Update(dt) != S_OK)
					return DanBiasClientReturn_Error;
				if(Render(dt) != S_OK)
					return DanBiasClientReturn_Error;

				prevTimeStamp = currTimeStamp;
			}
		}
		return DanBiasClientReturn_Sucess;
	}

	void DanBiasGame::Release()
	{
		CleanUp();
	}


	//--------------------------------------------------------------------------------------
	// Register class and create window
	//--------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitWindow( HINSTANCE hInstance, int nCmdShow )
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
	// Create Direct3D with Oyster Graphics
	//--------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitDirect3D()
	{
		if(Oyster::Graphics::API::Init(g_hWnd, false, false, Oyster::Math::Float2( 1024, 768)) != Oyster::Graphics::API::Sucsess)
			return E_FAIL;
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Init the input
	//-------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitInput()
	{
		m_data->inputObj = new InputClass;
		if(!m_data->inputObj->Initialize(g_hInst, g_hWnd, 1024, 768))
		{
			MessageBox(0, L"Could not initialize the input object.", L"Error", MB_OK);
			return E_FAIL;
		}
		return S_OK;
	}
	
	HRESULT DanBiasGame::Update(float deltaTime)
	{
		m_data->inputObj->Update();

		DanBias::Client::GameClientState::ClientState state = DanBias::Client::GameClientState::ClientState_Same;
		state = m_data->recieverObj->gameClientState->Update(deltaTime, m_data->inputObj);

		if(state != Client::GameClientState::ClientState_Same)
		{
			m_data->recieverObj->gameClientState->Release();
			delete m_data->recieverObj->gameClientState;
			m_data->recieverObj->gameClientState = NULL;

			switch (state)
			{
			case Client::GameClientState::ClientState_Lobby:
				m_data->recieverObj->gameClientState = new Client::LobbyState();
				break;
			case Client::GameClientState::ClientState_Game:
				m_data->recieverObj->gameClientState = new Client::GameState();
				break;
			default:
				return E_FAIL;
				break;
			}
			m_data->recieverObj->gameClientState->Init(m_data->recieverObj->nwClient); // send game client
				 
		}
		return S_OK;
	}

	HRESULT DanBiasGame::Render(float deltaTime)
	{
		int isPressed = 0;
		if(m_data->inputObj->IsKeyPressed(DIK_A))
		{
			isPressed = 1;
		}
		
		wchar_t title[255];
		swprintf(title, sizeof(title), L"| Pressing A:  %d | \n", (int)(isPressed));
		SetWindowText(g_hWnd, title);
	
		m_data->recieverObj->gameClientState->Render();

		return S_OK;
	}

	HRESULT DanBiasGame::CleanUp()
	{
		m_data->recieverObj->gameClientState->Release();
		delete m_data->recieverObj->gameClientState;
		m_data->recieverObj->nwClient->Disconnect();
		delete m_data->recieverObj->nwClient;
		delete m_data->recieverObj;
		delete m_data->inputObj;
		delete m_data;
		

		Oyster::Graphics::API::Clean();
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

} //End namespace DanBias
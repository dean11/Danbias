#define NOMINMAX
#include <Windows.h>
#include "Include\DanBiasGame.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameClientState/GameClientState.h"
#include "GameClientState\GameState.h"
#include "GameClientState\LobbyState.h"
#include <GameProtocols.h>
#include "NetworkClient.h"

#include "../WindowManager/WindowShell.h"
#include "L_inputClass.h"
#include "WinTimer.h"
#include "vld.h"

namespace DanBias
{

#pragma region Game Data


	struct MyRecieverObject :public Oyster::Network::ProtocolRecieverObject
	{
	Oyster::Network::NetworkClient* nwClient;
	Client::GameClientState* gameClientState;
	
	void NetworkCallback(Oyster::Network::CustomNetProtocol& p) override
	{
		int pType = p[0].value.netInt;
		switch (pType)
		{
		case protocol_General_Status:
			{
				GameLogic::Protocol_General_Status::States state;
				state =  (GameLogic::Protocol_General_Status::States)p[1].value.netShort;

			}
			break;
		case protocol_Gameplay_PlayerNavigation:
			{
				Client::GameClientState::KeyInput* protocolData = new Client::GameClientState::KeyInput;
				for(int i = 0; i< 6; i++)
				{
					protocolData->key[i] = p[i+1].value.netBool;
				}

				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(protocolData);
				delete protocolData;
				protocolData = NULL;
			}
			break;
		case protocol_Gameplay_PlayerPosition:
			{
				Client::GameClientState::PlayerPos* protocolData = new Client::GameClientState::PlayerPos;
				for(int i = 0; i< 3; i++)
				{
					protocolData->playerPos[i] = p[i].value.netFloat;
				}
				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(protocolData);
				delete protocolData;
				protocolData = NULL;
			}
			break;

		case protocol_Gameplay_CreateObject:
			{

				Client::GameClientState::NewObj* protocolData = new Client::GameClientState::NewObj;
				protocolData->object_ID = p[1].value.netInt;
				protocolData->path = p[2].value.netCharPtr;
				for(int i = 0; i< 16; i++)
				{
					protocolData->worldPos[i] = p[i+3].value.netFloat;
				}

				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(protocolData);

				delete protocolData;
				protocolData = NULL;
			}
			break;
		case protocol_Gameplay_RemoveObject:
			{
				Client::GameClientState::RemoveObj* protocolData = new Client::GameClientState::RemoveObj;
				protocolData->object_ID = p[1].value.netInt;

				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(protocolData);

				delete protocolData;
				protocolData = NULL;
			}
			break;
		case protocol_Gameplay_ObjectPosition:
			{
				Client::GameClientState::ObjPos protocolData;
				protocolData.object_ID = p[1].value.netInt;
				for(int i = 0; i< 16; i++)
				{
					protocolData.worldPos[i] = p[i+2].value.netFloat;
				}

				if(dynamic_cast<Client::GameState*>(gameClientState))
					((Client::GameState*)gameClientState)->Protocol(&protocolData);
			
				//Why use dynamicly allocated memory when data dies after block?
				//Client::GameClientState::ObjPos* protocolData = new Client::GameClientState::ObjPos;
				//protocolData->object_ID = p[1].value.netInt;
				//for(int i = 0; i< 16; i++)
				//{
				//	protocolData->worldPos[i] = p[i+2].value.netFloat;
				//}
				//
				//if(dynamic_cast<Client::GameState*>(gameClientState))
				//	((Client::GameState*)gameClientState)->Protocol(protocolData);
				//
				//delete protocolData;
				//protocolData = NULL;
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
		WindowShell* window;
		InputClass* inputObj;
		Utility::WinTimer* timer;
		MyRecieverObject* recieverObj;

	} data;
#pragma endregion


	DanBiasGamePrivateData* DanBiasGame::m_data = new DanBiasGamePrivateData();

	//--------------------------------------------------------------------------------------
	// Interface API functions
	//--------------------------------------------------------------------------------------
	DanBiasClientReturn DanBiasGame::Initiate(DanBiasGameDesc& desc)
	{

		if(! m_data->window->CreateWin(WindowShell::WINDOW_INIT_DESC()))
			return DanBiasClientReturn_Error;

		if( FAILED( InitDirect3D() ) )
			return DanBiasClientReturn_Error;

		if( FAILED( InitInput() ) )
			return DanBiasClientReturn_Error;

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
		if(!m_data->recieverObj->gameClientState->Init(m_data->recieverObj->nwClient))
			return DanBiasClientReturn_Error;

		 m_data->timer = new Utility::WinTimer();
		 m_data->timer->reset();
		return DanBiasClientReturn_Sucess;
	}

	DanBiasClientReturn DanBiasGame::Run()
	{
		// Main message loop
		while(m_data->window->Frame())
		{
			float dt = (float)m_data->timer->getElapsedSeconds();
			m_data->timer->reset();

			if(Update(dt) != S_OK)
				return DanBiasClientReturn_Error;
			if(Render(dt) != S_OK)
				return DanBiasClientReturn_Error;
		}
		return DanBiasClientReturn_Sucess;
	}

	void DanBiasGame::Release()
	{
		CleanUp();
	}

	//--------------------------------------------------------------------------------------
	// Create Direct3D with Oyster Graphics
	//--------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitDirect3D()
	{
		if(Oyster::Graphics::API::Init(m_data->window->GetHWND(), false, false, Oyster::Math::Float2( 1024, 768)) != Oyster::Graphics::API::Sucsess)
			return E_FAIL;
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Init the input
	//-------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitInput() 
	{
		m_data->inputObj = new InputClass;
		if(!m_data->inputObj->Initialize(m_data->window->GetHINSTANCE(), m_data->window->GetHWND(), m_data->window->GetHeight(), m_data->window->GetWidth()))
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
		SetWindowText(m_data->window->GetHWND(), title);
	
		m_data->recieverObj->gameClientState->Render();

		return S_OK;
	}

	HRESULT DanBiasGame::CleanUp()
	{
		m_data->recieverObj->gameClientState->Release();
		delete m_data->recieverObj->gameClientState;
		m_data->recieverObj->nwClient->Disconnect();
		delete m_data->recieverObj->nwClient;
		delete m_data->timer;
		delete m_data->recieverObj;
		delete m_data->inputObj;
		delete m_data;
		

		Oyster::Graphics::API::Clean();
		return S_OK;
	}	

} //End namespace DanBias
#define NOMINMAX
#include <Windows.h>
#include "Include\DanBiasGame.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameClientState/GameClientState.h"
#include "GameClientState\GameState.h"
#include "GameClientState\LobbyState.h"
#include "GameClientState\LoginState.h"
#include "GameClientState\LanMenuState.h"
#include <Protocols.h>
#include "NetworkClient.h"

#include "../WindowManager/WindowShell.h"
#include "L_inputClass.h"
#include "WinTimer.h"
#include "vld.h"
#include "GameClientRecieverFunc.h"

namespace DanBias
{

#pragma region Game Data

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
		GameRecieverObject* recieverObj;

	} data;
#pragma endregion


	DanBiasGamePrivateData* DanBiasGame::m_data = new DanBiasGamePrivateData();
	float DanBiasGame::capFrame = 0;

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

		m_data->recieverObj = new GameRecieverObject;
		/*m_data->recieverObj->Connect(desc.port, desc.IP);

		if (!m_data->recieverObj->IsConnected())
		{
			// failed to connect
			return DanBiasClientReturn_Error;
		}*/
		// Start in lobby state
		m_data->recieverObj->gameClientState = new  Client::LoginState();
		if(!m_data->recieverObj->gameClientState->Init(m_data->recieverObj))
			return DanBiasClientReturn_Error;

		m_data->timer = new Utility::WinTimer(); //why dynamic memory?
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

			capFrame += dt;
			if(capFrame > 0.03)
			{
				if(Update(dt) != S_OK)
					return DanBiasClientReturn_Error;
				if(Render(dt) != S_OK)
					return DanBiasClientReturn_Error;
				capFrame = 0; 
			}

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
			m_data->recieverObj->gameClientState->Init(m_data->recieverObj); // send game client
				 
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
		m_data->recieverObj->Disconnect();
		delete m_data->recieverObj;
		delete m_data->timer;
		delete m_data->inputObj;
		delete m_data;
		

		Oyster::Graphics::API::Clean();
		return S_OK;
	}	

} //End namespace DanBias
#define NOMINMAX
#include <Windows.h>
#include "Include\DanBiasGame.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameClientState/GameClientState.h"
#include "GameClientState\GameState.h"
#include "GameClientState\LobbyState.h"
#include "GameClientState\MainState.h"
#include "GameClientState\LanMenuState.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include <GameServerAPI.h>

#include "../WindowManager/WindowShell.h"
#include "L_inputClass.h"
#include "WinTimer.h"
#include "vld.h"

#include "../Misc/EventHandler/EventHandler.h"

using namespace ::Oyster::Event;
using namespace Oyster::Network;
using namespace ::Utility::DynamicMemory;

void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e );

namespace DanBias
{
#pragma region Game Data
	class DanBiasGamePrivateData
	{

	public:
		WindowShell* window;
		InputClass* inputObj;
		Utility::WinTimer timer;
		UniquePointer<Client::GameClientState> state;
		NetworkClient networkClient;
		bool serverOwner;

		float capFrame;

		DanBiasGamePrivateData()
		{
			this->capFrame = 0;
		}
	} data;
#pragma endregion


	//--------------------------------------------------------------------------------------
	// Interface API functions
	//--------------------------------------------------------------------------------------
	DanBiasClientReturn DanBiasGame::Initiate(DanBiasGameDesc& desc)
	{

		WindowShell::CreateConsoleWindow();
		//if(! data.window->CreateWin(WindowShell::WINDOW_INIT_DESC(L"Window", cPOINT(1600, 900), cPOINT())))
		if(! data.window->CreateWin(WindowShell::WINDOW_INIT_DESC()))
			return DanBiasClientReturn_Error;

		if( FAILED( InitDirect3D() ) )
			return DanBiasClientReturn_Error;

		if( FAILED( InitInput() ) )
			return DanBiasClientReturn_Error;

		data.serverOwner = false;

		data.networkClient.SetMessagePump( ClientEventFunction );

		// Start in main menu state
		data.state = new Client::MainState();

		if( !data.state->Init( &data.networkClient ) )
			return DanBiasClientReturn_Error;

		data.timer.reset();
		return DanBiasClientReturn_Sucess;
	}

	DanBiasClientReturn DanBiasGame::Run()
	{
		// Main message loop
		while(data.window->Frame())
		{
			float dt = (float)data.timer.getElapsedSeconds();
			data.timer.reset();

			::Oyster::Graphics::API::Update( dt );

			if(data.networkClient.IsConnected())
				data.networkClient.Update();
			
			data.capFrame += dt;
			if(data.capFrame > 0.03)
			{
				if(Update(dt) != S_OK)
					return DanBiasClientReturn_Error;
				if(Render() != S_OK)
					return DanBiasClientReturn_Error;
				data.capFrame = 0; 
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
		if(Oyster::Graphics::API::Init(data.window->GetHWND(), false, false, Oyster::Math::Float2( 1024, 768)) != Oyster::Graphics::API::Sucsess)
			return E_FAIL;
		Oyster::Graphics::API::Option p;
		p.modelPath = L"..\\Content\\Models\\";
		p.texturePath = L"..\\Content\\Textures\\";
		Oyster::Graphics::API::SetOptions(p);
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Init the input
	//-------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitInput() 
	{
		data.inputObj = new InputClass;
		if(!data.inputObj->Initialize(data.window->GetHINSTANCE(), data.window->GetHWND(), data.window->GetHeight(), data.window->GetWidth()))
		{
			MessageBox(0, L"Could not initialize the input object.", L"Error", MB_OK);
			return E_FAIL;
		}
		return S_OK;
	}
	
	HRESULT DanBiasGame::Update(float deltaTime)
	{
		data.inputObj->Update();

		if(data.serverOwner)
		{
			DanBias::GameServerAPI::ServerUpdate();
		}

		DanBias::Client::GameClientState::ClientState state = DanBias::Client::GameClientState::ClientState_Same;

		state = data.state->Update( deltaTime, data.inputObj );

		if(state != Client::GameClientState::ClientState_Same)
		{
			bool stateVal = false;
			data.state->Release();

			switch (state)
			{
			case Client::GameClientState::ClientState_LobbyCreated:
				data.serverOwner = true;
				stateVal = true;
			case Client::GameClientState::ClientState_Lobby:
				data.state = new Client::LobbyState();
				stateVal = true;
				break;
			case Client::GameClientState::ClientState_Game:
				
				break;
			default:
				return E_FAIL;
				break;
			}

			if(stateVal)
			{
				data.state->Init( &data.networkClient ); // send game client
			}
			else
			{

			}
				 
		}
		return S_OK;
	}

	HRESULT DanBiasGame::Render( )
	{
		data.state->Render();

		return S_OK;
	}

	HRESULT DanBiasGame::CleanUp()
	{
		data.networkClient.Disconnect();
		delete data.inputObj;

		Oyster::Graphics::API::Clean();

		GameServerAPI::ServerStop();

		return S_OK;
	}	

} //End namespace DanBias

void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	if( DanBias::data.state )
		DanBias::data.state->DataRecieved( e );
}

#define NOMINMAX
#include <Windows.h>
#include "Include\DanBiasGame.h"
#include "DllInterfaces/GFXAPI.h"
#include "GameClientState/GameClientState.h"
#include "GameClientState\GameState.h"
#include "GameClientState\LobbyState.h"
#include "GameClientState\LobbyAdminState.h"
#include "GameClientState\MainState.h"
#include "GameClientState\LanMenuState.h"
#include "GameClientState\NetLoadState.h"
#include <Protocols.h>
#include "NetworkClient.h"
#include <GameServerAPI.h>

#include "../WindowManager/WindowShell.h"
#include "L_inputClass.h"
#include "WinTimer.h"
#include "vld.h"

#include "EventHandler/EventHandler.h"

#include "GameClientState\SharedStateContent.h"

using namespace ::Oyster;
using namespace ::Oyster::Event;
using namespace ::Oyster::Network;
using namespace ::Utility::DynamicMemory;
using namespace ::DanBias::Client;

void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e );

namespace DanBias
{
#pragma region Game Data
	class DanBiasGamePrivateData
	{
	public:
		WindowShell* window;
		InputClass inputObj;
		Utility::WinTimer timer;

		UniquePointer<Client::GameClientState> state;
		SharedStateContent sharedStateContent;
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

		data.sharedStateContent.network = &data.networkClient;
		data.sharedStateContent.input	= &data.inputObj;

		// Start in main menu state
		data.state = new Client::MainState();

		if( !data.state->Init( data.sharedStateContent ) )
			return DanBiasClientReturn_Error;

		data.timer.reset();
		return DanBiasClientReturn_Success;
	}

	DanBiasClientReturn DanBiasGame::Run()
	{
		// Main message loop
		while(data.window->Frame())
		{
			float dt = (float)data.timer.getElapsedSeconds();
			data.timer.reset();

			Graphics::API::Update( dt );
			
			data.capFrame += dt;
			if(data.capFrame > 0.03)
			{
				switch( Update(dt) )
				{
				case Result_continue:	break;
					case Result_quit:	return DanBiasClientReturn_Success;
				case Result_error:		return DanBiasClientReturn_Error;
				default:				break;
				}
				if(Render() != S_OK)
					return DanBiasClientReturn_Error;
				data.capFrame = 0; 
			}

			if(data.networkClient.IsConnected())
				data.networkClient.Update();

		}
		return DanBiasClientReturn_Success;
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
		
		Oyster::Graphics::API::Option p;
		p.modelPath = L"..\\Content\\Models\\";
		p.texturePath = L"..\\Content\\Textures\\";
		p.Resolution = Oyster::Math::Float2( 1024, 768);
		//! @todo fix proper amb value
		p.AmbientValue = 1.0f;

		if(Oyster::Graphics::API::Init(data.window->GetHWND(), false, false, p) != Oyster::Graphics::API::Sucsess)
			return E_FAIL;
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Init the input
	//-------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitInput() 
	{
		if(!data.inputObj.Initialize(data.window->GetHINSTANCE(), data.window->GetHWND(), data.window->GetHeight(), data.window->GetWidth()))
		{
			MessageBox(0, L"Could not initialize the input object.", L"Error", MB_OK);
			return E_FAIL;
		}
		return S_OK;
	}
	
	DanBiasGame::Result DanBiasGame::Update(float deltaTime)
	{
		{ // updating mouse input
			POINT mousePos;
			GetCursorPos( &mousePos );

			RECT windowVertex;
			GetWindowRect( data.window->GetHWND(), &windowVertex );

			float mouseNormalisedX = (float)(mousePos.x - windowVertex.left);
			mouseNormalisedX /= (float)(windowVertex.right - windowVertex.left);

			float mouseNormalisedY = (float)(mousePos.y - windowVertex.top);
			mouseNormalisedY /= (float)(windowVertex.bottom - windowVertex.top);

			data.inputObj.Update( mouseNormalisedX, mouseNormalisedY );
		}		

		if( data.serverOwner )
		{
			DanBias::GameServerAPI::ServerUpdate();
		}

		DanBias::Client::GameClientState::ClientState state = DanBias::Client::GameClientState::ClientState_Same;

		state = data.state->Update( deltaTime );

		if( state != Client::GameClientState::ClientState_Same )
		{
			bool stateChanged = false;
			data.state->Release();

			switch (state)
			{
			case Client::GameClientState::ClientState_Main:
				data.state = new Client::MainState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Lan:
				data.state = new Client::LanMenuState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Lobby:
				data.state = new Client::LobbyState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_LobbyCreate:
				data.state = new Client::LobbyAdminState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Game:
				data.state = new Client::GameState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_NetLoad:
				data.state = new Client::NetLoadState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Quit:
				data.state->Release();
				return Result_quit;
			default:
				data.state->Release();
				return Result_error;
			}

			if( stateChanged )
			{
				data.state->Init( data.sharedStateContent ); // send game client
			}			 
		}
		return Result_continue;
	}

	HRESULT DanBiasGame::Render( )
	{
		data.state->Render();

		return S_OK;
	}

	HRESULT DanBiasGame::CleanUp()
	{
		if( data.networkClient.IsConnected() )
			data.networkClient.Disconnect();

		data.state = nullptr;
		EventHandler::Instance().Clean();
		Graphics::API::Clean();

		GameServerAPI::ServerStop();

		return S_OK;
	}	

} //End namespace DanBias

void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	if( DanBias::data.state )
		DanBias::data.state->DataRecieved( e );
}

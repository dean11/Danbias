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
#include "GameClientState\States\OptionState.h"
#include "Utilities.h"
#include <Protocols.h>
#include "NetworkClient.h"
//#include <GameServerAPI.h>

#include "../WindowManager/WindowShell.h"
#include "WinTimer.h"

#include "EventHandler/EventHandler.h"

#include "GameClientState/SharedStateContent.h"
#include "Utilities.h"

using namespace ::Oyster;
using namespace ::Oyster::Event;
using namespace ::Oyster::Network;
using namespace ::Utility::DynamicMemory;
using namespace ::DanBias::Client;
using namespace ::Utility::DynamicMemory;

LRESULT CALLBACK WindowCallBack(HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e );

#pragma region Game Data
namespace DanBias
{
	class DanBiasGamePrivateData
	{
	public:
		WindowShell* window;
		Utility::WinTimer timer;

		UniquePointer<Client::GameClientState> state;
		NetworkClient networkClient;

		SharedStateContent sharedStateContent;

		bool serverOwner;
		float capFrame_gameplay, capFrame_gfx;

		DanBiasGamePrivateData()
		{
			this->sharedStateContent.network			= nullptr;
			this->sharedStateContent.mouseDevice		= nullptr;
			this->sharedStateContent.keyboardDevice		= nullptr;
			this->sharedStateContent.mouseSensitivity	= Utility::Value::Radian( 0.6f );
			this->serverOwner							= false;
			this->capFrame_gameplay						= 0;
			this->capFrame_gfx							= 0;
		}

		~DanBiasGamePrivateData()
		{
		}
	} data;
}
#pragma endregion

namespace DanBias
{
	//--------------------------------------------------------------------------------------
	// Interface API functions
	//--------------------------------------------------------------------------------------
	DanBiasClientReturn DanBiasGame::Initiate( DanBiasGameDesc& desc )
	{
		WindowShell::CreateConsoleWindow();
		//if(! data.window->CreateWin(WindowShell::WINDOW_INIT_DESC(L"Window", cPOINT(1600, 900), cPOINT())))

		WindowShell::WINDOW_INIT_DESC winDesc;
		winDesc.windowSize.x		= 1200;
		winDesc.windowSize.y		= 800;
		winDesc.windowProcCallback	= WindowCallBack;

		if(! data.window->CreateWin(winDesc) )
			return DanBiasClientReturn_Error;

		if( FAILED( InitInput(data.window->GetHWND()) ) )
			return DanBiasClientReturn_Error;

		if( FAILED( InitDirect3D() ) )
			return DanBiasClientReturn_Error;

		data.serverOwner = false;

		data.networkClient.SetMessagePump( ClientEventFunction );

		data.sharedStateContent.network = &data.networkClient;

		if( (data.sharedStateContent.background = Graphics::API::CreateTexture( L"color_white.png" ) ) == 0)
			printf("Failed to load the default background [color_white.png]\n");

		
		if( ( data.sharedStateContent.mouseCursor = Graphics::API::CreateTexture( L"cursor.png" ) ) == 0)
			printf("Failed to load the mouse cursor [cursor.png]\n");

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
			static const float gameplay_frame_periodicy = 1.0f / 30.0f,
							   gfx_frame_periodicy = 1.0f / 60.0f;

			float dt = (float)data.timer.getElapsedSeconds();
			data.timer.reset();

			
			data.capFrame_gameplay += dt;
			data.capFrame_gfx += dt;

			if( data.capFrame_gameplay >= gameplay_frame_periodicy )
			{
				switch( Update(gameplay_frame_periodicy) )
				{
				case Result_continue:	break;
					case Result_quit:	return DanBiasClientReturn_Success;
				case Result_error:		return DanBiasClientReturn_Error;
				default:				break;
				}
				data.capFrame_gameplay -= gameplay_frame_periodicy; 
			}

			if( data.capFrame_gfx >= gfx_frame_periodicy )
			{
				Graphics::API::Update( gfx_frame_periodicy );
				if(Render() != S_OK)
					return DanBiasClientReturn_Error;
				data.capFrame_gfx -= gfx_frame_periodicy; 
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
		Oyster::Graphics::API::Option gfxOp;
		gfxOp.modelPath = L"..\\Content\\Models\\";
		gfxOp.texturePath = L"..\\Content\\Textures\\";
		gfxOp.resolution = Oyster::Math::Float2( 1280.0f, 720.0f );
		gfxOp.ambientValue = 0.2f;
		gfxOp.fullscreen = false;
		gfxOp.globalGlowTint = Math::Float3(2.0f, 2.0f, 2.0f);
		gfxOp.globalTint = Math::Float3(1.0f, 1.0f, 1.0f);

		if(Oyster::Graphics::API::Init(data.window->GetHWND(), false, gfxOp) != Oyster::Graphics::API::Sucsess)
			return E_FAIL;
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Init the input
	//-------------------------------------------------------------------------------------
	HRESULT DanBiasGame::InitInput( HWND handle ) 
	{
		data.sharedStateContent.mouseDevice = dynamic_cast<Input::Mouse*>( ::Input::InputManager::Instance()->CreateDevice(Input::Enum::SAIType_Mouse, handle) );
		if( !data.sharedStateContent.mouseDevice )
		{
			MessageBox( 0, L"Could not initialize the mouseDevice.", L"Error", MB_OK );
			return E_FAIL;
		}
		
		data.sharedStateContent.keyboardDevice= dynamic_cast<Input::Keyboard*>( ::Input::InputManager::Instance()->CreateDevice(Input::Enum::SAIType_Keyboard, handle) );
		if( !data.sharedStateContent.keyboardDevice )
		{
			MessageBox( 0, L"Could not initialize the raw keyboard device.", L"Error", MB_OK );
			return E_FAIL;
		}

		return S_OK;
	}
	
	DanBiasGame::Result DanBiasGame::Update(float deltaTime)
	{
		/*if( data.serverOwner )
		{
		DanBias::GameServerAPI::ServerUpdate();
		}
		*/
		DanBias::Client::GameClientState::ClientState state = DanBias::Client::GameClientState::ClientState_Same;
		state = data.state->Update( deltaTime );

		if( state != Client::GameClientState::ClientState_Same )
		{
			bool stateChanged = false;
			
			switch (state)
			{
			case Client::GameClientState::ClientState_Main:
				data.networkClient.Disconnect();
				data.state->Release();
				data.state = new Client::MainState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Lan:
				data.state->Release();
				data.state = new Client::LanMenuState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Lobby:
				data.state->Release();
				data.state = new Client::LobbyState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_LobbyCreate:
				data.state->Release();
				data.state = new Client::LobbyAdminState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Game:
				data.state->Release();
				data.state = new Client::GameState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_NetLoad:
				data.state->Release();
				data.state = new Client::NetLoadState();
				stateChanged = true;
				break;
			case Client::GameClientState::ClientState_Options:
				data.state->Release();
				data.state = new Client::OptionState();
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

		Input::InputManager::DestroyInputManager();
		EventHandler::Instance().Clean();
		Graphics::API::Clean();

		//GameServerAPI::ServerStop();

		return S_OK;
	}	

} //End namespace DanBias

LRESULT CALLBACK WindowCallBack(HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message ) 
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
		break;
		case WM_INPUT:
			message = 0;
		break;
		default: break;
	}

	return DefWindowProc( handle, message, wParam, lParam );
}

void ClientEventFunction( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	if( DanBias::data.state )
		DanBias::data.state->DataRecieved( e );
}

#include "LobbyAdminState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include <Protocols.h>

#include "EventHandler\EventHandler.h"
#include "Buttons\ButtonRectangle.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::Oyster::Event;
using namespace ::Oyster::Math3D;

struct LobbyAdminState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	Graphics::API::Texture background;
	EventButtonCollection button;
} privData;

void OnButtonInteract_Ready( Oyster::Event::ButtonEvent<GameClientState*>& e );

LobbyAdminState::LobbyAdminState(void) {}

LobbyAdminState::~LobbyAdminState(void)
{
	if( this->privData )
		this->Release();
}

bool LobbyAdminState::Init(NetworkClient* nwClient)
{
	privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;

	this->privData->background = Graphics::API::CreateTexture( L"grass_md.png" );

	// create buttons
	ButtonRectangle<GameClientState*> *button;
	
	button = new ButtonRectangle<GameClientState*>( L"earth_md.png", OnButtonInteract_Ready, this, 0.5f, 0.2f, 0.3f, 0.1f, true );
	this->privData->button.AddButton( button );

	// bind button collection to the singleton eventhandler
	EventHandler::Instance().AddCollection( &this->privData->button );

	return true;
}

GameClientState::ClientState LobbyAdminState::Update(float deltaTime, InputClass* KeyInput)
{
	// Wishlist:
	// picking 
	// mouse events
	// different menus
	// play sounds
	// update animation
	// send data to server
	// check data from server

	MouseInput mouseState;
	{
		mouseState.x = KeyInput->GetPitch();
		mouseState.y = KeyInput->GetYaw();
		mouseState.mouseButtonPressed = KeyInput->IsMousePressed();
	}

	EventHandler::Instance().Update( mouseState );

	return this->privData->nextState;
}
bool LobbyAdminState::Render( )
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float2(0.5f), Float2(1.0f) );
	this->privData->button.Render();

	Graphics::API::EndFrame();
	return true;
}
bool LobbyAdminState::Release()
{
	privData = NULL;
	return true;
}

void LobbyAdminState::ChangeState( ClientState next )
{
	if( next == GameClientState::ClientState_LobbyReady )
	{ // If all is ready start server

	}
	else
		this->privData->nextState = next;
}

using namespace ::Oyster::Network;

void LobbyAdminState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	CustomNetProtocol data = e.args.data.protocol;
	short ID = data[0].value.netShort; // fetching the id data.
	
	// Block irrelevant messages.
	if( ProtocolIsLobby(ID) )
	{
		switch(ID)
		{
		case protocol_Lobby_Create:		break; /** @todo TODO: implement */
		case protocol_Lobby_Start:		break; /** @todo TODO: implement */
		case protocol_Lobby_Join:		break; /** @todo TODO: implement */
		case protocol_Lobby_Login:		break; /** @todo TODO: implement */
		case protocol_Lobby_Refresh:	break; /** @todo TODO: implement */
		case protocol_Lobby_ClientData:	break; /** @todo TODO: implement */
		case protocol_Lobby_GameData:	break; /** @todo TODO: implement */
		default:						break;
		}
	}
	else if( ProtocolIsGeneral(ID) )
	{
		switch( ID )
		{
			case protocol_General_Status:				break; /** @todo TODO: implement */
			case protocol_General_Text:					break; /** @todo TODO: implement */
		default: break;
		}
	}
}

void OnButtonInteract_Ready( Oyster::Event::ButtonEvent<GameClientState*>& e )
{
	switch( e.state )
	{
	case ButtonState_Released:
		e.owner->ChangeState( GameClientState::ClientState_LobbyReady );
		break;
	default: break;
	}
}
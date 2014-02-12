#include "LobbyState.h"
#include "DllInterfaces/GFXAPI.h"
#include "OysterMath.h"
#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include <GameServerAPI.h>
#include <Protocols.h>

using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::DanBias::Client;

struct LobbyState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
} privData;

LobbyState::LobbyState(void) {}

LobbyState::~LobbyState(void)
{
	if( this->privData )
		this->Release();
}

bool LobbyState::Init(NetworkClient* nwClient)
{
	privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;

	return true;
}

GameClientState::ClientState LobbyState::Update(float deltaTime, InputClass* KeyInput)
{
	//// picking 
	//// mouse events
	//// different menus
	//// play sounds
	//// update animation
	//// send data to server
	//// check data from server

	//if(GameServerAPI::ServerIsRunning() && GameServerAPI::ServerIsRunning())	//May be a problem if server is not shut down properly after lan session.
	//{
	//	if( KeyInput->IsKeyPressed(DIK_G)) 
	//	{
	//		if(!DanBias::GameServerAPI::GameStart())
	//		{
	//			
	//		}
	//	}
	//}
	  
	//return ClientState_Same;

	return this->privData->nextState;
}
bool LobbyState::Render( )
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	Graphics::API::EndFrame();
	return true;
}
bool LobbyState::Release()
{
	privData = NULL;
	return true;
}

void LobbyState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

using namespace ::Oyster::Network;

void LobbyState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
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
#include "LanMenuState.h"

#include "C_obj/C_Player.h"
#include "C_obj/C_StaticObj.h"
#include "C_obj/C_DynamicObj.h"
#include "DllInterfaces/GFXAPI.h"

#include "LobbyState.h"
#include "GameState.h"
#include "../Network/NetworkAPI/NetworkClient.h"

#include <GameServerAPI.h>

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;

struct  LanMenuState::MyData
{
	MyData(){}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;

} privData;

LanMenuState::LanMenuState() {}

LanMenuState::~LanMenuState() {}

bool LanMenuState::Init(Network::NetworkClient* nwClient)
{
	privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;

	return true;
}

GameClientState::ClientState LanMenuState::Update(float deltaTime, InputClass* KeyInput)
{
	/*ChangeState(KeyInput);

	if(privData->recieverObj->IsConnected())
		privData->recieverObj->Update();
	KeyInput->Update();

	if(privData->serverOwner)
	{
		DanBias::GameServerAPI::ServerUpdate();
	}

	DanBias::Client::GameClientState::ClientState state = DanBias::Client::GameClientState::ClientState_Same;
	state = privData->recieverObj->gameClientState->Update(deltaTime, KeyInput);

	if(state != Client::GameClientState::ClientState_Same)
	{
		privData->recieverObj->gameClientState->Release();
		delete privData->recieverObj->gameClientState;
		privData->recieverObj->gameClientState = NULL;

		switch (state)
		{
		case Client::GameClientState::ClientState_LobbyCreated:
			privData->serverOwner = true;
		case Client::GameClientState::ClientState_Lobby:
			privData->recieverObj->gameClientState = new Client::LobbyState();
			break;
		case Client::GameClientState::ClientState_Game:
			privData->recieverObj->gameClientState = new Client::GameState();
			break;
		default:
			//return E_FAIL;
			break;
		}
		privData->recieverObj->gameClientState->Init(privData->recieverObj); // send game client
				 
	}*/

	//return ChangeState(KeyInput);
	return this->privData->nextState;
}

//GameClientState::ClientState LanMenuState::ChangeState(InputClass* KeyInput)
//{
//	// create game
//	if( KeyInput->IsKeyPressed(DIK_C)) 
//	{
//		DanBias::GameServerAPI::ServerInitDesc desc; 
//
//		DanBias::GameServerAPI::ServerInitiate(desc);
//		DanBias::GameServerAPI::ServerStart();
//		// my ip
//		nwClient->Connect(15151, "127.0.0.1");
//
//		if (!nwClient->IsConnected())
//		{
//			// failed to connect
//			return ClientState_Same;
//		}
//		return ClientState_Lobby;
//	}
//	// join game
//	if( KeyInput->IsKeyPressed(DIK_J)) 
//	{
//		// game ip
//		nwClient->Connect(15151, "194.47.150.56");
//
//		if (!nwClient->IsConnected())
//		{
//			// failed to connect
//			return ClientState_Same;
//		}
//		return ClientState_Lobby;
//	}
//	return ClientState_Same;
//}

bool LanMenuState::Render( )
{
	Graphics::API::NewFrame();

	Graphics::API::StartGuiRender();



	Graphics::API::EndFrame();
	return true;
}

bool LanMenuState::Release()
{

	privData = NULL;
	return true;
}

void LanMenuState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}
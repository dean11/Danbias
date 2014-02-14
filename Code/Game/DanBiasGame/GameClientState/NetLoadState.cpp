#include "NetLoadState.h"
#include "NetworkClient.h"
#include "../Game/GameProtocols/Protocols.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;
using namespace ::GameLogic;

struct NetLoadState::MyData
{
	MyData() {}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	bool loading;
};

NetLoadState::NetLoadState(void) {}

NetLoadState::~NetLoadState(void)
{
	if( this->privData )
		this->Release();
}

bool NetLoadState::Init( NetworkClient* nwClient )
{
	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = nwClient;
	this->privData->loading = false;

	// we may assume that nwClient is properly connected to the server
	// signals querry to server for loading instructions
	nwClient->Send( Protocol_QuerryGameType() );

	return true;
}

GameClientState::ClientState NetLoadState::Update(float deltaTime, InputClass* KeyInput)
{
	return this->privData->nextState;
}

bool NetLoadState::Render()
{
	return true;
}

bool NetLoadState::Release()
{
	if( this->privData )
	{
		this->privData = NULL;
	}
	return true;
}

void NetLoadState::ChangeState( ClientState next )
{
	this->privData->nextState = next;
}

void NetLoadState::DataRecieved( NetEvent<NetworkClient*, NetworkClient::ClientEventArgs> e )
{
	// fetching the id data.
	short ID = e.args.data.protocol[0].value.netShort;
	
	if( ID == protocol_Lobby_CreateGame && !this->privData->loading )
	{
		this->LoadGame( Protocol_LobbyCreateGame(e.args.data.protocol).modelName );
	}
}

void NetLoadState::LoadGame( const ::std::string &fileName )
{
	this->privData->loading = true;

	// TODO: ask Sam about level loader

	this->privData->nextState = ClientState::ClientState_Game;
}

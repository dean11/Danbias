#include "NetLoadState.h"
#include "NetworkClient.h"
#include "OysterMath.h"
#include "../Game/GameProtocols/Protocols.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Math;
using namespace ::Oyster::Network;
using namespace ::GameLogic;

struct NetLoadState::MyData
{
	MyData() {}

	GameClientState::ClientState nextState;
	NetworkClient *nwClient;
	Graphics::API::Texture background;
	bool loading;
};

NetLoadState::NetLoadState(void) {}

NetLoadState::~NetLoadState(void)
{
	if( this->privData )
		this->Release();
}

bool NetLoadState::Init( SharedStateContent &shared )
{
	this->privData = new MyData();

	this->privData->nextState = GameClientState::ClientState_Same;
	this->privData->nwClient = shared.network;
	this->privData->background = Graphics::API::CreateTexture( L"grass_md.png" );
	this->privData->loading = false;

	// we may assume that nwClient is properly connected to the server
	// signals querry to server for loading instructions
	this->privData->nwClient->Send( Protocol_QuerryGameType() );

	return true;
}

GameClientState::ClientState NetLoadState::Update( float deltaTime )
{
	return this->privData->nextState;
}

bool NetLoadState::Render()
{
	Graphics::API::NewFrame();
	Graphics::API::StartGuiRender();

	Graphics::API::RenderGuiElement( this->privData->background, Float3(0.5f, 0.5f, 1.0f), Float2(1.0f) );

	Graphics::API::EndFrame();

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

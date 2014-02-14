#include "NetLoadState.h"
#include "NetworkClient.h"
#include "../Game/GameProtocols/ProtocolIdentificationID.h"

using namespace ::DanBias::Client;
using namespace ::Oyster;
using namespace ::Oyster::Network;

struct NetLoadState::MyData
{
	MyData() {}

	GameClientState::ClientState nextState;
	//NetworkClient *nwClient; needed?
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
	//this->privData->nwClient = nwClient; needed?

	// we may assume that nwClient is properly connected to the server


	this->privData->nextState = GameClientState::ClientState_Main;

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
	CustomNetProtocol data = e.args.data.protocol;
	short ID = data[0].value.netShort; // fetching the id data.
	
	//if( ProtocolIsGameplay(ID) )
	//{
	//	switch(ID)
	//	{
	//	//case protocol_Gameplay_ObjectPickup:			break; /** @todo TODO: implement */
	//	default: break;
	//	}
	//}
	//else if( ProtocolIsGeneral(ID) )
	//{
	//	switch( ID )
	//	{
	//		case protocol_General_Status:				break; /** @todo TODO: implement */
	//		case protocol_General_Text:					break; /** @todo TODO: implement */
	//	default: break;
	//	}
	//}
}
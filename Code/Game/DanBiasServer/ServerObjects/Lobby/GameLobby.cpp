#include "GameLobby.h"


namespace DanBias
{
	GameLobby::GameLobby(Utility::DynamicMemory::SmartPointer<ClientObject> owner)
	{

	}
	GameLobby::~GameLobby()
	{

	}
	void GameLobby::Release()
	{

	}
	void GameLobby::Join(Utility::DynamicMemory::SmartPointer<ClientObject> client)
	{
		NetworkSession::AttachClient(client);
	}

	void GameLobby::AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box) 
	{ }
}//End namespace DanBias
#ifndef DANBIASSERVER_GAMELOBBY_H
#define DANBIASSERVER_GAMELOBBY_H

#include "..\NetworkSession.h"

namespace DanBias
{
	class GameLobby :public NetworkSession
	{
	public:
		GameLobby(Utility::DynamicMemory::SmartPointer<ClientObject> owner);
		virtual~GameLobby();

		void Release();

		void Join(Utility::DynamicMemory::SmartPointer<ClientObject> client);

	private:
		void AttachClient(Utility::DynamicMemory::SmartPointer<ClientObject> client, Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box = 0) override;

	};
}//End namespace DanBias

#endif // !DANBIASSERVER_GAME_LOBBY_H
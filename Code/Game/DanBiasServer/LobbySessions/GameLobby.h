/////////////////////////////////////////////////////////////////////
// Created by [Dennis Andersen] [2013]
/////////////////////////////////////////////////////////////////////
#ifndef DANBIASSERVER_GAMELOBBY_H
#define DANBIASSERVER_GAMELOBBY_H

#include "NetworkSession.h"

namespace DanBias
{
	class GameLobby :public NetworkSession
	{
	public:
		GameLobby(Utility::DynamicMemory::SmartPointer<LobbyClient> owner);
		virtual~GameLobby();

		void Release();

	private:
		friend class AdminInterface;
	};
}//End namespace DanBias

#endif // !DANBIASSERVER_GAME_LOBBY_H
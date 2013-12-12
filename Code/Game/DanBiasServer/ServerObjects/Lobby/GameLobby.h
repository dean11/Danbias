#ifndef DANBIASSERVER_GAME_LOBBY_H
#define DANBIASSERVER_GAME_LOBBY_H

#include "..\NetworkSession.h"

namespace DanBias
{
	class GameLobby :public NetworkSession
	{
	public:
		GameLobby();
		~GameLobby();
		void Release();

	private:


	};
}//End namespace DanBias

#endif // !DANBIASSERVER_GAME_LOBBY_H
#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include "..\NetworkSession.h"
#include <PostBox\IPostBox.h>

namespace DanBias
{
	class MainLobby :public NetworkSession
	{
	public:
		MainLobby();
		virtual~MainLobby();
		void Release();

		void Frame();

	private:
		void ParseEvents();

	private:
		Oyster::IPostBox<DanBias::NetworkSession::NetEvent> *box;
		
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H

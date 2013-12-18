#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include "..\NetworkSession.h"

namespace DanBias
{
	class MainLobby :public NetworkSession
	{
	public:
		MainLobby();
		~MainLobby();
		void Release();

		void Frame();

	private:
		void ParseEvents();

	private:
		//NetworkSession *
	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H

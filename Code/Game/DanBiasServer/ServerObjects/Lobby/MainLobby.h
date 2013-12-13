#ifndef DANBIASSERVER_MAINLOBBY_H
#define DANBIASSERVER_MAINLOBBY_H

#include "..\NetworkSession.h"
#include <thread>
#include <future>

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


	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H

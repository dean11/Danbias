#ifndef DANBIASGAME_GAMELOBBY_H
#define DANBIASGAME_GAMELOBBY_H

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

	private:


	};
}//End namespace DanBias
#endif // !DANBIASGAME_GAMELOBBY_H

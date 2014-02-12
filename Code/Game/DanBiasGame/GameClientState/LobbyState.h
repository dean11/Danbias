#ifndef DANBIAS_CLIENT_LOBBYSTATE_H
#define DANBIAS_CLIENT_LOBBYSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "NetworkClient.h"
#include <string>
namespace DanBias
{
	namespace Client
	{		
		class LobbyState : public GameClientState
		{
		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		public:
			LobbyState(void);
			~LobbyState(void);
			bool Init(Oyster::Network::NetworkClient* nwClient);
			ClientState Update(float deltaTime, InputClass* KeyInput);
			// create session lobby
			// join session lobby
			// set name
			// set rules
			// set map
			// ready 
			// chat
			// kick

			bool Render();
			bool Release();
			void ChangeState( ClientState next );

			void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );
		};
	}
}
#endif // ! DANBIAS_CLIENT_GAMECLIENTSTATE_H

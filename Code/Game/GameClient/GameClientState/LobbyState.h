#ifndef DANBIAS_CLIENT_LOBBYSTATE_H
#define DANBIAS_CLIENT_LOBBYSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "NetworkClient.h"
#include <string>

// Feature wishlist:
// create session lobby
// join session lobby
// set name
// set rules
// set map
// ready 
// chat
// kick

namespace DanBias
{
	namespace Client
	{		
		class LobbyState : public GameClientState
		{
		public:
			LobbyState();
			~LobbyState();

			bool Init( SharedStateContent &shared );
			ClientState Update( float deltaTime );
			bool Render();
			bool Release();
			void ChangeState( ClientState next );
			void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );
			const NetEvent & DataRecieved( const NetEvent &message );

		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		};
	}
}
#endif // ! DANBIAS_CLIENT_GAMECLIENTSTATE_H

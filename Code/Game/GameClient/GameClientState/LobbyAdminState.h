#ifndef DANBIAS_CLIENT_LOBBYADMINSTATE_H
#define DANBIAS_CLIENT_LOBBYADMINSTATE_H

#include "GameClientState.h"
#include "NetworkClient.h"

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
		class LobbyAdminState : public GameClientState
		{
		public:
			LobbyAdminState();
			~LobbyAdminState();

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

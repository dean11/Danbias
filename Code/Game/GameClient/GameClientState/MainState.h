#ifndef DANBIAS_CLIENT_MAINSTATE_H
#define DANBIAS_CLIENT_MAINSTATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "NetworkClient.h"
#include <string>
namespace DanBias
{
	namespace Client
	{
		class MainState : public GameClientState
		{
		public:
			MainState();
			~MainState();
			bool Init( SharedStateContent &shared );
			ClientState Update( float deltaTime );
			
			bool Render();
			bool Release();
			void ChangeState( ClientState next );
			void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );
		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		};
	}
}
#endif // ! DANBIAS_CLIENT_LOGINSTATE_H
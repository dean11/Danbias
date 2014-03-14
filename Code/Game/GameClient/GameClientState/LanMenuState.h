#ifndef LAN_MENU_STATE_H
#define LAN_MENU_STATE_H

#include "GameClientState.h"
#include "OysterMath.h"
#include "Keyboard.h"
#include <string>
#include <DynamicArray.h>

namespace DanBias
{
	namespace Client
	{
		class LanMenuState : public GameClientState, Input::Keyboard::KeyboardEvent
		{
		public:
			LanMenuState();
			virtual ~LanMenuState();

			bool Init( SharedStateContent &shared );
			virtual ClientState Update( float deltaTime );

			virtual bool Render();
			virtual bool Release();
			void ChangeState( ClientState next );
			void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );

			virtual const NetEvent & DataRecieved( const NetEvent &message );

			void OnKeyPress	(Input::Enum::SAKI key, Input::Keyboard* sender) override;

		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		
		};
	}
}

#endif
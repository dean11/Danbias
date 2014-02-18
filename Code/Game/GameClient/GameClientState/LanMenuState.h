#ifndef LAN_MENU_STATE_H
#define LAN_MENU_STATE_H

#include "GameClientState.h"
#include "OysterMath.h"

namespace DanBias
{
	namespace Client
	{
		class LanMenuState : public GameClientState
		{
		public:
			LanMenuState();
			virtual ~LanMenuState();

			bool Init( SharedStateContent &shared );
			virtual ClientState Update( float deltaTime );

			virtual bool Render();
			virtual bool Release();
			void ChangeState( ClientState next );

		void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );

		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;
		};
	}
}

#endif
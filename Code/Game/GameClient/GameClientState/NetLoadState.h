#ifndef DANBIAS_CLIENT_NETLOADSTATE_H
#define DANBIAS_CLIENT_NETLOADSTATE_H

#include "GameClientState.h"
#include "NetworkClient.h"

namespace DanBias
{
	namespace Client
	{
		class NetLoadState : public GameClientState
		{
		public:
			NetLoadState( );
			virtual ~NetLoadState( );

			bool Init( SharedStateContent &shared );
			ClientState Update( float deltaTime );
			
			bool Render();
			bool Release();
			void ChangeState( ClientState next );

			void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );
		
		private:
			struct MyData;
			::Utility::DynamicMemory::UniquePointer<MyData> privData;

			void LoadGame( const ::std::string &fileName );
		};
	}
}
#endif // ! DANBIAS_CLIENT_LOGINSTATE_H
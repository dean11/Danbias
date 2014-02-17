#ifndef DANBIAS_CLIENT_GAMECLIENTSTATE_H
#define DANBIAS_CLIENT_GAMECLIENTSTATE_H

#define NOMINMAX
#include "L_inputClass.h"
#include "NetworkClient.h"

namespace DanBias { namespace Client
{
	class GameClientState
	{
	public:
		enum ClientState
		{
			ClientState_Main,
			ClientState_Lan,
			ClientState_Lobby,
			ClientState_LobbyCreate,
			ClientState_LobbyReady,
			ClientState_NetLoad,
			ClientState_Game,
			ClientState_Same,
			ClientState_Quit
		};

	public:
		GameClientState(void);
		virtual ~GameClientState(void);
		virtual bool Init(Oyster::Network::NetworkClient* nwClient) = 0;
		virtual ClientState Update(float deltaTime, InputClass* KeyInput) = 0;
		virtual bool Render() = 0;
		virtual bool Release() = 0;
		virtual void ChangeState( ClientState next ) = 0;

		virtual void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );
	};
} }

namespace Utility { namespace DynamicMemory
{ // template specializationto allowuse of dynamicmemory tools
	template<>
	inline void SafeDeleteInstance( ::DanBias::Client::GameClientState *dynamicInstance )
	{
		if( dynamicInstance )
		{
			dynamicInstance->Release();
			delete dynamicInstance;
		}
	}
} }

#endif
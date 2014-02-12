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
			ClientState_Login,
			ClientState_Lobby,
			ClientState_Lan,
			ClientState_LobbyCreated,
			ClientState_Game,
			ClientState_Same,
		};

	public:
		GameClientState(void);
		virtual ~GameClientState(void);
		virtual bool Init(Oyster::Network::NetworkClient* nwClient) = 0;
		virtual ClientState Update(float deltaTime, InputClass* KeyInput) = 0;
		virtual bool Render() = 0;
		virtual bool Release() = 0;

		virtual void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );
	};
} }
#endif
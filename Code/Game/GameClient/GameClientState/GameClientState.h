#ifndef DANBIAS_CLIENT_GAMECLIENTSTATE_H
#define DANBIAS_CLIENT_GAMECLIENTSTATE_H

#define NOMINMAX
#include "SharedStateContent.h"

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
			ClientState_ResetGame,
			ClientState_Options,
			ClientState_Same,
			ClientState_Quit
		};

		typedef ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> NetEvent;
		static const NetEvent event_processed;

		GameClientState();
		virtual ~GameClientState();
		virtual bool Init( SharedStateContent &shared ) = 0;
		virtual ClientState Update( float deltaTime ) = 0;
		virtual bool Render() = 0;
		virtual bool Release() = 0;
		virtual void ChangeState( ClientState next ) = 0;
		//virtual void PlaySound( Sound::ISound* sound, Sound::IChannel* channel, PlayMode playMode ) = 0;
		//virtual void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode ) = 0;

		/******************************************************************
		 * @param message of the event
		 * @return message or a reference to GameClientState::event_processed.
		 ******************************************************************/
		virtual const NetEvent & DataRecieved( const NetEvent &message );
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
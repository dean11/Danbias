#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>

namespace DanBias { namespace Client
{
	class GameState : public GameClientState
	{
	public:
		enum gameStateState
		{
			gameStateState_loading,
			gameStateState_playing,
			gameStateState_end,
		};

		GameState(void);
		~GameState(void);
		bool Init( SharedStateContent &shared );
		GameClientState::ClientState Update( float deltaTime ) override;
		void InitiatePlayer( int id, const std::string &modelName, const float position[3], const float rotation[4], const float scale[3], bool isMyPlayer );
		void ReadKeyInput();
		bool Render()override;
		bool Release()override;
		void ChangeState( ClientState next );

		void DataRecieved( ::Oyster::Network::NetEvent<::Oyster::Network::NetworkClient*, ::Oyster::Network::NetworkClient::ClientEventArgs> e );

	private:
		struct MyData;
		::Utility::DynamicMemory::UniquePointer<MyData> privData;
	};
} }
#endif
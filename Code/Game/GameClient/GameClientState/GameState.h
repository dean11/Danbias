#ifndef DANBIAS_CLIENT_GAMESTATE_H
#define DANBIAS_CLIENT_GAMESTATE_H
#include "GameClientState.h"
#include "OysterMath.h"
#include <string>
#include "GameStateUI.h"
#include "C_obj\C_Player.h"


namespace DanBias { namespace Client
{
	class GameState : public GameClientState, Input::Keyboard::KeyboardEvent
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
		void InitiatePlayer( int id, const std::string &alias, const std::string &modelName, const float position[3], const float rotation[4], const float scale[3], bool isMyPlayer );
		void ReadKeyInput();
		bool Render()override;
		bool Release()override;
		void ChangeState( ClientState next );
		void PlaySound( SoundID soundID, ChannelID channelID, PlayMode playMode );
		void PlaySoundCollisionSound( SoundID soundID, PlayMode playMode, float* pos );
		void PlayPlayerSound( SoundID soundID, int playerId, PlayerSoundID playerSoundID, PlayMode playMode );

		const NetEvent & DataRecieved( const NetEvent &message );

	private:
		struct MyData;
		::Utility::DynamicMemory::UniquePointer<MyData> privData;
		GameStateUI *uiStack[4], *gameUI, *respawnUI, *statsUI, *inGameMeny, *inGameOptions;
		int uiStackTop;

		// DEGUG KEYS
		bool key_Reload_Shaders;
		bool key_Wireframe_Toggle; 
		bool renderWhireframe; 
		bool key_showStats;
		bool renderStats;
		bool gameOver; 
		// !DEGUG KEYS

		void OnKeyPress		(Input::Enum::SAKI key, Input::Keyboard* sender) override;
		void OnKeyRelease	(Input::Enum::SAKI key, Input::Keyboard* sender) override;

		void Gameplay_ObjectPickup( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectDamage( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectHealthStatus( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectPosition( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectScale( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectRotation( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectPositionRotation( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectEnabled( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectDisabled( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectCreate( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectCreatePlayer( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectJoinTeam( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectLeaveTeam( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectWeaponCooldown( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectWeaponEnergy( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectRespawn( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectDie( Oyster::Network::CustomNetProtocol data );
		void Gameplay_PlayerScore( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectDisconnectPlayer( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectAction( Oyster::Network::CustomNetProtocol data );
		void Gameplay_ObjectCollision( Oyster::Network::CustomNetProtocol data );
		void Gameplay_EffectCreateBeam( Oyster::Network::CustomNetProtocol data );
		void General_GameOver( Oyster::Network::CustomNetProtocol data );
		void General_Timer( Oyster::Network::CustomNetProtocol data );
		//:HACK!
		//void OnMouseMoveVelocity ( Input::Struct::SAIPointInt2D coordinate, Input::Mouse* sender ) override;
		//void SetUp( DanBias::Client::C_Player* p);

		void UIstackPush( GameStateUI *ui );
		GameStateUI * UIstackPop();
		void UIstackRemove( GameStateUI *ui );
		GameStateUI * UIstackPeek();
		void UIstackSet( GameStateUI *ui );
		void UIstackClear();
		GameStateUI::UIState UIstackUpdate( float deltaTime );
		void UIstackRenderGUI();
		void UIstackRenderText();
	};
} }
#endif
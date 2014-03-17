/////////////////////////////////////////////////////////////////////
// Created by [Erik Persson] [2013]
/////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H

//Includes windows so we need to undef minmax
#define NOMINMAX
#include <vld.h>

#include "GameAPI.h"
#include "Player.h"
#include "Level.h"

#include <DynamicArray.h>
#include <GID.h>
#include <PhysicsAPI.h>
#include <WinTimer.h>

namespace GameLogic
{
	class Game	:public GameAPI
	{
	
	public:
		class PlayerData :public IPlayerData
		{
		public:
			PlayerData();
			PlayerData(int playerID,int teamID);
			~PlayerData();

			void Move(const PLAYER_MOVEMENT &movement)													override;
			void UseWeapon(const WEAPON_FIRE &usage)													override;
			void SelectWeaponAttatchment( int socketID )												override;
			int GetTeamID() const																		override;
			PLAYER_STATE GetState() const																override;
			Oyster::Math::Float3 GetPosition()															override;
			Oyster::Math::Quaternion GetRotation()														override;
			Oyster::Math::Float3 GetScale()																override;
			Oyster::Math::Float4x4 GetOrientation()														override;
			bool IsDisabled() const																		override;
			int GetID() const																			override;
			int GetKills() const override;
			int GetDeaths() const override;
			void SetLookDir(const Oyster::Math3D::Float3& lookDir)		override;
			void TurnLeft(Oyster::Math3D::Float deltaLeftRadians )										override;
			ObjectSpecialType GetObjectType()	const													override;
			void Inactivate()																			override;
			void Release()																				override;
			Player* GetPlayer();
			Player *player;
		};

		class LevelData	:public ILevelData
		{
		public:
			LevelData();
			~LevelData();
			Oyster::Math::Float3 GetPosition()						override;
			Oyster::Math::Quaternion GetRotation()					override;
			Oyster::Math::Float3 GetScale()							override;
			Oyster::Math::Float4x4 GetOrientation()					override; 
			bool IsDisabled() const									override;
			int GetID() const										override;
			ObjectSpecialType GetObjectType()	const				override;
			int getNrOfDynamicObj()const							override;
			IObjectData* GetObjectAt(int ID) const					override;
			void GetAllDynamicObjects(Utility::DynamicMemory::DynamicArray<IObjectData*>& mem) const override;
			void Update(float deltaTime);
			void AddPlayerToGame(IPlayerData *player);
			void RemovePlayerFromGame(IPlayerData *player);
			void InitGameMode(float maxTimeSec, int endKillScore);
			Level *level;
		};

	public:
		Game();
		~Game();

		void GetAllPlayerPositions() const																				override;
		PlayerData* CreatePlayer()																						override;
		void RemovePlayer( IPlayerData* )																				override;
		LevelData* CreateLevel(const wchar_t mapName[255] )																override;
		void Release()																									override;
		void CreateTeam()																								override;
		bool NewFrame(float delta )																						override;
		void SetFPS( int FPS )																							override;
		void SetFrameTimeLength( float seconds )																		override;
		void SetMoveSubscription(GameEvent::ObjectMovedFunction functionPointer) 										override;
		void SetDisableSubscription(GameEvent::ObjectDisabledFunction functionPointer) 									override;
		void SetEnableSubscription(GameEvent::ObjectEnabledFunction functionPointer) 									override;
		void SetHpSubscription(GameEvent::ObjectHpFunction functionPointer) 											override;
		void SetRespawnSubscription(GameEvent::ObjectRespawnedFunction functionPointer) 								override;
		void SetDeadSubscription(GameEvent::ObjectDeadFunction functionPointer) 										override;
		void SetActionSubscription(GameEvent::AnimationEventFunction functionPointer) 									override;
		void SetPickupSubscription(GameEvent::PickupEventFunction functionPointer) 										override;
		void SetCollisionSubscription(GameEvent::CollisionEventFunction functionPointer) 								override;
		void SetWeaponEnergySubscription(GameEvent::WeaponEnergyFunction functionPointer)								override;
		void SetGameOverSubscription(GameEvent::EndGameFunction functionPointer)										override;
		void SetBeamEffectSubscription(GameEvent::BeamEffectFunction functionPointer)									override;
		void SetOnGameTimeTick(GameEvent::OnGameTimeTickFunction functionPointer, float perdiod)						override;
		bool Initiate()																									override;

		float GetFrameTime() const;

		static void PhysicsOnMove(const Oyster::Physics::ICustomBody *object);
		static void PhysicsOnDestroy(::Utility::DynamicMemory::UniquePointer<Oyster::Physics::ICustomBody> proto);
		static void PhysicsOnDead(const Oyster::Physics::ICustomBody *object);

		Utility::DynamicMemory::DynamicArray<PlayerData*> players;
		LevelData* level;
		float frameTime;
		float timer;
		bool initiated;
		float tickPeriod;
		float tickTimer;

		GameEvent::ObjectMovedFunction		onMoveFnc;
		GameEvent::ObjectDisabledFunction	onDisableFnc;
		GameEvent::ObjectEnabledFunction	onEnableFnc;
		GameEvent::ObjectHpFunction			onDamageTakenFnc;
		GameEvent::ObjectRespawnedFunction 	onRespawnFnc;
		GameEvent::ObjectDeadFunction		onDeadFnc;
		GameEvent::AnimationEventFunction	onActionEventFnc;
		GameEvent::PickupEventFunction		onPickupEventFnc;
		GameEvent::CollisionEventFunction	onCollisionEventFnc;
		GameEvent::WeaponEnergyFunction		onEnergyUpdateFnc;
		GameEvent::EndGameFunction			onEndGameFnc;
		GameEvent::BeamEffectFunction		onBeamEffectFnc;
		GameEvent::OnGameTimeTickFunction	onGameTimeTickFnc;
	};	
}


#endif

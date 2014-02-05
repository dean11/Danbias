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

			void Move(const PLAYER_MOVEMENT &movement)				override;
			void UseWeapon(const WEAPON_FIRE &usage)				override;
			int GetTeamID() const									override;
			PLAYER_STATE GetState() const							override;
			Oyster::Math::Float3 GetPosition()						override;
			Oyster::Math::Float4x4 GetOrientation()					override; 
			int GetID() const										override;
			OBJECT_TYPE GetObjectType()	const						override;
			void Rotate(const Oyster::Math3D::Float4 lookDir)		override;

			Player *player;
		};

		class LevelData	:public ILevelData
		{
		public:
			LevelData();
			~LevelData();
			Oyster::Math::Float3 GetPosition()						override;
			Oyster::Math::Float4x4 GetOrientation()					override; 
			int GetID() const										override;
			OBJECT_TYPE GetObjectType()	const						override;
			int getNrOfDynamicObj()const							override;
			IObjectData* GetObjectAt(int ID) const					override;
			Level *level;
		};

	public:
		Game();
		~Game();

		void GetAllPlayerPositions() const																				override;
		PlayerData* CreatePlayer()																						override;
		LevelData* CreateLevel()																						override;
		void CreateTeam()																								override;
		bool NewFrame()																									override;
		void SetFPS( int FPS )																							override;
		void SetFrameTimeLength( float seconds )																		override;
		void SetSubscription(GameEvent::ObjectEventFunctionType type, GameEvent::ObjectEventFunction functionPointer)	override;
		bool Initiate()																									override;

		float GetFrameTime() const;

		static void PhysicsOnMove(const Oyster::Physics::ICustomBody *object);
		static void PhysicsOnDestroy(::Utility::DynamicMemory::UniquePointer<Oyster::Physics::ICustomBody> proto);

		Utility::DynamicMemory::DynamicArray<PlayerData*> players;
		LevelData* level;
		float frameTime;
		bool initiated;
		GameEvent::ObjectEventFunction onDisableFnc;
		GameEvent::ObjectEventFunction onMoveFnc;

	};	
}


#endif

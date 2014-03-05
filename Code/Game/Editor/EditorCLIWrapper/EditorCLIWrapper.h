#ifndef CLISTANDALONESERVER_CPP_Standalone_CLI_H
#define CLISTANDALONESERVER_CPP_Standalone_CLI_H

#include <windows.h>

#include "..\NoEdgeEditorWrapper\NoEdgeEditorWrapper.h"


namespace System { namespace Windows { namespace Interop
{
	namespace EnumBridge
	{
		public enum class NoEdgeType
		{
			NoEdgeType_World,
			NoEdgeType_Projectile,
			NoEdgeType_Interactive,
			NoEdgeType_Building,
			NoEdgeType_HazardEnv,
			NoEdgeType_Light,
			NoEdgeType_Pickup,
		};
	
		public enum class NoEdgeType_Projectiles
		{
			Stone				= NoEdgeType_Projectiles_Stone,
			StandardBox			= NoEdgeType_Projectiles_StandardBox,
			SpikeBox			= NoEdgeType_Projectiles_SpikeBox,
			Spike				= NoEdgeType_Projectiles_Spike,
			CrystalShard		= NoEdgeType_Projectiles_CrystalShard,
		};
		public enum class NoEdgeType_Interactive
		{
			SpawnPoint			= NoEdgeType_Interactive_SpawnPoint,
			JumpPad				= NoEdgeType_Interactive_JumpPad,
			Portal				= NoEdgeType_Interactive_Portal,
			ExplosiveBox		= NoEdgeType_Interactive_ExplosiveBox,
		};
		public enum class NoEdgeType_Buildings
		{
			CorporateBuilding	= NoEdgeType_Buildings_Building1,
			Platform			= NoEdgeType_Buildings_Building2,
		};
		public enum class NoEdgeType_Light
		{
			PointLight				= NoEdgeType_Light_PointLight,
		};
		public enum class NoEdgeType_HazardEnv
		{
			CrystalFormation1		= NoEdgeType_HazardEnv_CrystalFormation1,
		};
		public enum class NoEdgeType_Pickup
		{
			HealthPackMedium	= NoEdgeType_Pickup_HealthPackMedium,
		};
	}

	public enum class EditorReturnCode
	{
		EditorReturnCode_Error,
		EditorReturnCode_Sucess,
	};
	
	public value struct InitDesc
	{
		value struct MainOptions
		{
			IntPtr renderWindow;
			int width;
			int height;
		} mainOptions;
	};

	public ref class GamePlanet
	{
	public:
		GamePlanet(NoEdgeEditorWrapper::NoEdgePlanet* e);
		~GamePlanet();
		!GamePlanet();

	public:
		void SetPosition(float v[3]);
		void SetRotation(float v[3]);
		void SetScale(float v[3]);
		void GetPosition(float v[3]);
		void GetRotation(float v[3]);
		void GetScale(float v[3]);

	public:
		void Release();

	private:
		NoEdgeEditorWrapper::NoEdgePlanet *planet;
	};

	public ref class GameEntity
	{
	public:
		GameEntity(NoEdgeEditorWrapper::NoEdgeEntity* e);
		~GameEntity();
		!GameEntity();

	public:
		void SetPosition(float v[3]);
		void SetRotation(float v[3]);
		void SetScale(float v[3]);
		void RelativeMove(float v[3]);
		void RelativeRotate(float v[3]);
		void RelativeScale(float v[3]);
		void Lock();
		void Unlock();
		void Hide();
		void Show();
		void GetPosition(float v[3]);
		void GetRotation(float v[3]);
		void GetScale(float v[3]);
		bool IsLocked();
		bool IsVisible();

	public:
		void Release();

	private:
		NoEdgeEditorWrapper::NoEdgeEntity *entity;
	};

	public ref class EditorCLIWrapper
	{
	private:

	public:
		EditorCLIWrapper();
		~EditorCLIWrapper();

		EditorReturnCode InitiateCore(InitDesc desc);

		bool LoadMap(String^ mapPath);

		void Frame();
		void Release();

		void OnResize( int width, int height );

		GamePlanet^ CreatePlanet();
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Buildings object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Interactive object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Light object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Projectiles object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Pickup object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_HazardEnv object);
	};

} } }
#endif
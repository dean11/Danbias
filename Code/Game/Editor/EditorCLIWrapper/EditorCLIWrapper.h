#ifndef CLISTANDALONESERVER_CPP_Standalone_CLI_H
#define CLISTANDALONESERVER_CPP_Standalone_CLI_H

#include <windows.h>

#include "..\NoEdgeEditorWrapper\NoEdgeEditorWrapper.h"


namespace System { namespace Windows { namespace Interop
{
	public enum class NoEdgeType_Collision
	{
		NoEdgeType_Collision_Box,
		NoEdgeType_Collision_Sphere,
		NoEdgeType_Collision_Cylinder,
	};
	namespace EnumBridge
	{
		public enum class NoEdgeType
		{
			NoEdgeType_World,
			NoEdgeType_Projectile,
			NoEdgeType_Interactive,
			NoEdgeType_Building,
			NoEdgeType_Light,
		};
	
		public enum class NoEdgeType_Projectiles
		{
			Stone			= NoEdgeType_Projectiles_Stone,
			StandardBox		= NoEdgeType_Projectiles_StandardBox,
			SpikeBox		= NoEdgeType_Projectiles_SpikeBox,
			Spike			= NoEdgeType_Projectiles_Spike,
			CrystalShard	= NoEdgeType_Projectiles_CrystalShard,
		};
		public enum class NoEdgeType_Interactive
		{
			SpawnPoint		= NoEdgeType_Interactive_SpawnPoint,
			JumpPad			= NoEdgeType_Interactive_JumpPad,
			Portal			= NoEdgeType_Interactive_Portal,
			RedExplosiveBox	= NoEdgeType_Interactive_RedExplosiveBox,
		};
		public enum class NoEdgeType_Buildings
		{
			Building			= NoEdgeType_Buildings_Building,
			CrystalFormation	= NoEdgeType_Buildings_CrystalFormation,
		};
		public enum class NoEdgeType_Light
		{
			PointLight				= NoEdgeType_Light_PointLight,
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

	public ref class GameWorld
	{
	public:
		GameWorld(NoEdgeEditorWrapper::NoEdgeWorld* e);
		~GameWorld();
		!GameWorld();

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
		NoEdgeEditorWrapper::NoEdgeWorld *world;
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

		GameWorld^ CreateWorld();
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Buildings object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Interactive object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Light object);
		GameEntity^ CreateEntity(EnumBridge::NoEdgeType_Projectiles object);
	};

} } }
#endif
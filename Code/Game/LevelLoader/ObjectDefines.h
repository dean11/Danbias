#ifndef OBJECT_DEFINES_H
#define OBJECT_DEFINES_H

#include <string>
#include <vector>

namespace GameLogic
{
	/************************************
				Enums
	*************************************/
	
	enum ObjectType
	{
		ObjectType_LevelMetaData,
		ObjectType_Static,
		ObjectType_Dynamic,
		ObjectType_Light,
		ObjectType_SpawnPoint,
		//Etc

		ObjectType_NUM_OF_TYPES,

		ObjectType_Unknown = -1
	};

	enum ObjectSpecialType
	{
		ObjectSpecialType_None,
		ObjectSpecialType_Generic,
		ObjectSpecialType_World,		//Always the main celestial body
		ObjectSpecialType_Building,
		ObjectSpecialType_Stone,
		ObjectSpecialType_StandardBox,
		ObjectSpecialType_RedExplosiveBox,
		ObjectSpecialType_SpikeBox,
		ObjectSpecialType_Spike,
		ObjectSpecialType_CrystalFormation,
		ObjectSpecialType_CrystalShard,
		ObjectSpecialType_JumpPad,
		ObjectSpecialType_Portal,
		ObjectSpecialType_PickupHealth,
		ObjectSpecialType_PickupEnergy,
		ObjectSpecialType_LightSource,
		ObjectSpecialType_NonSolidRotationSlow,
		ObjectSpecialType_NonSolidRotationQuick,

		

		
		ObjectSpecialType_Player,

		ObjectSpecialType_Count,
		ObjectSpecialType_Unknown  = -1
	};

	enum CollisionGeometryType
	{
		CollisionGeometryType_Box,
		CollisionGeometryType_Sphere,
		CollisionGeometryType_Cylinder,
		CollisionGeometryType_CG_MESH,

		CollisionGeometryType_Count,
		CollisionGeometryType_Unknown = -1
	};

	//Only supports Pointlight right now.
	enum LightType
	{
		LightType_PointLight,
		//LightType_DirectionalLight,
		//LightType_SpotLight,

		LightType_Count,
		LightType_Unknown = -1
	};

	//Should this be moved somewhere else?
	enum GameMode
	{
		GameMode_FreeForAll,
		GameMode_TeamDeathMatch,
		//Etc

		GameMode_Count,
		GameMode_Unknown = -1
	};

	enum WorldSize
	{
		WorldSize_Tiny,
		WorldSize_Small,
		WorldSize_Medium,
		WorldSize_Big,
		WorldSize_Humongous,

		WorldSize_Count,
		WorldSize_Unknown = -1
	};

	enum PlayerAction
	{
		PlayerAction_Jump = 0,
		PlayerAction_Walk = 1,
		PlayerAction_Idle = 2,
	};
	// continue ID counting from playerAction
	enum WeaponAction
	{
		WeaponAction_PrimaryShoot		= 3,
		WeaponAction_SecondaryShoot		= 4,
		WeaponAction_UtilityActivate	= 5,
		WeaponAction_GunShoot			= 6, 
		WeaponAction_EnergyDepleted		= 7,

	};

	// TODO: add more collision Events
	enum CollisionEvent
	{
		CollisionEvent_BoxVsBox,
		CollisionEvent_BoxVsPlayer,
		CollisionEvent_CrystalVsPlayer,
		CollisionEvent_JumpPad,
		CollisionEvent_Explosion,
		CollisionEvent_Portal,
	};
	enum PickupType
	{
		PickupType_Health,
		PickupType_SpeedBoost
	};
	enum PlayerHealthEvent
	{
		PlayerHealthEvent_Damage, 
		PlayerHealthEvent_Heal,
		PlayerHealthEvent_Respawn,
		PlayerHealthEvent_Join,
	};

	/************************************
				Structs
	*************************************/
	namespace LevelLoaderInternal
	{
		struct FormatVersion
		{
			unsigned int formatVersionMajor;
			unsigned int formatVersionMinor;
			
			FormatVersion()
				: formatVersionMajor(0), formatVersionMinor(0)
			{}

			FormatVersion(unsigned int major, unsigned int minor)
				: formatVersionMajor(major), formatVersionMinor(minor)
			{}

			bool operator ==(const FormatVersion& obj)
			{
				return (this->formatVersionMajor == obj.formatVersionMajor && this->formatVersionMinor == obj.formatVersionMinor);
			}

			bool operator !=(const FormatVersion& obj)
			{
				return !(*this == obj);
			}
		};
	}

	struct ObjectTypeHeader
	{
		ObjectType typeID;

		//Unless this is here the object destructor wont be called.
		virtual ~ObjectTypeHeader(){}
	};

	namespace LevelLoaderInternal
	{
		const FormatVersion boundingVolumeVersion(2, 0);

		struct BoundingVolumeBase
		{
			CollisionGeometryType geoType;
			float position[3];
			float rotation[4];
			float frictionCoeffStatic;
			float frictionCoeffDynamic;
			float restitutionCoeff;
			float mass;
		};

		struct BoundingVolumeBox : public BoundingVolumeBase
		{
			float size[3];
		};

		struct BoundingVolumeSphere : public BoundingVolumeBase
		{
			float radius;
		};

		struct BoundingVolumeCylinder : public BoundingVolumeBase
		{
			float length;
			float radius;
		};

		struct BoundingVolumeCGMesh : public BoundingVolumeBase
		{
			wchar_t filename[128];
		};

		struct BoundingVolume
		{
			CollisionGeometryType geoType;
			union
			{
				LevelLoaderInternal::BoundingVolumeBox box;
				LevelLoaderInternal::BoundingVolumeSphere sphere;
				LevelLoaderInternal::BoundingVolumeCylinder cylinder;
				LevelLoaderInternal::BoundingVolumeCGMesh cgMesh;
			};
		};
	}

	struct LevelMetaData : public ObjectTypeHeader
	{
		std::string levelName;
		unsigned int levelVersion;
		std::string levelDescription;
		std::string levelAuthor;
		unsigned int maxNumberOfPlayer;
		WorldSize worldSize;
		std::string overviewPicturePath;
		std::vector<GameMode> gameModesSupported;
		
		virtual ~LevelMetaData(){}

	};

	struct ObjectHeader : public ObjectTypeHeader
	{
		//Special type id for special objects: portal, jumppad, exploding objects, etc.
		ObjectSpecialType specialTypeID;
		//Model,
		std::string ModelFile;
		//Position
		float position[3];
		//Rotation Quaternion
		float rotation[4];
		//Scale
		float scale[3];

		::GameLogic::LevelLoaderInternal::BoundingVolume boundingVolume;

		virtual ~ObjectHeader(){}
	};

	//inheritance from the base class because there is no use for ModelFile, Rotation and Scale
	//so this is a special struct for just spawnpoints
	struct SpawnPointAttributes : public ObjectTypeHeader
	{
		float position[3];
	};

	/************************************
				Special objects
	*************************************/

	struct JumpPadAttributes : public ObjectHeader
	{
		float direction[3];
		float power;
	};

	struct PortalAttributes : public ObjectHeader
	{
		float destination[3];
	};

	struct WorldAttributes : public ObjectHeader
	{
		float worldSize;
		float atmoSphereSize;
	};

	struct SkyAttributes : public ObjectHeader
	{
		float skySize;
	};
	

	/************************************
				Lights
	*************************************/

	struct BasicLight : public ObjectTypeHeader
	{
		LightType lightType;	//Is not used right now
		float color[3];
		float position[3];
		float radius;
		float intensity;

		virtual ~BasicLight(){}
	};
	/* We only support pointlight right now.
	struct PointLight : public BasicLight
	{
		float position[3];
	};

	struct DirectionalLight : public BasicLight
	{
		float direction[3];
	};

	struct SpotLight : public BasicLight
	{
		float direction[3];
		float range;
		float attenuation[3];
	};*/
}

#endif
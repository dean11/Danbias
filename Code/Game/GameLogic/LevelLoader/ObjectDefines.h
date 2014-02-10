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
		//Etc

		ObjectType_NUM_OF_TYPES,

		ObjectType_Unknown = -1
	};

	enum ObjectSpecialType
	{
		ObjectSpecialType_World,		//Always the main celestial body
		ObjectSpecialType_Building,
		ObjectSpecialType_Damaging,
		ObjectSpecialType_Explosive,
		ObjectSpecialType_JumpPad,
		ObjectSpecialType_BoostPad,
		ObjectSpecialType_Portal,
		ObjectSpecialType_SpawnPoint,

		ObjectSpecialType_Count,
		ObjectSpecialType_Unknown  = -1
	};

	enum UsePhysics
	{
		UsePhysics_UseFullPhysics,
		UsePhysics_IgnoreGravity,
		UsePhysics_IgnorePhysics,
		UsePhysics_IgnoreCollision,

		UsePhysics_Count,
		UsePhysics_Unknown = -1
	};

	enum CollisionGeometryType
	{
		CollisionGeometryType_Box,
		CollisionGeometryType_Sphere,
		CollisionGeometryType_Cylinder,

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
		const FormatVersion boundingVolumeVersion(1, 0);

		struct BoundingVolumeBase
		{
			float position[3];
		};

		struct BoundingVolumeBox : public BoundingVolumeBase
		{
			float size[3];
			float angularAxis[3];
			float angle;
		};

		struct BoundingVolumeSphere : public BoundingVolumeBase
		{
			float radius;
		};

		struct BoundingVolumeCylinder : public BoundingVolumeBase
		{
			float length;
			float angularAxis[3];
			float angle;
			float radius;
		};

		struct BoundingVolume
		{
			CollisionGeometryType geoType;
			union
			{
				LevelLoaderInternal::BoundingVolumeBox box;
				LevelLoaderInternal::BoundingVolumeSphere sphere;
				LevelLoaderInternal::BoundingVolumeCylinder cylinder;
			};
		};

		struct PhysicsObject
		{
			UsePhysics usePhysics;
			float mass;
			float inertiaMagnitude[3];
			float inertiaRotation[3];
			float frictionCoeffStatic;
			float frictionCoeffDynamic;
			float restitutionCoeff;
			BoundingVolume boundingVolume;
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

	struct ObjectHeader : public ObjectTypeHeader, public LevelLoaderInternal::PhysicsObject
	{
		//Special type id for special objects: portal, jumppad, exploding objects, etc.
		ObjectSpecialType specialTypeID;
		//Model,
		std::string ModelFile;
		//Position
		float position[3];
		//Rotation
		float rotation[3];
		float angle;
		//Scale
		float scale[3];

		virtual ~ObjectHeader(){}
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

	struct SpawnPointAttributes : public ObjectHeader
	{
		float spawnPosition[3];
	};

	/************************************
				Lights
	*************************************/

	struct BasicLight : public ObjectTypeHeader
	{
		LightType lightType;	//Is not used right now
		float color[3];
		float position[3];
		float raduis;
		float intensity;
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
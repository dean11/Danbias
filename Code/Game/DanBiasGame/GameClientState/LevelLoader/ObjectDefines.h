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

		CollisionGeometryType_Count,
		CollisionGeometryType_Unknown = -1
	};

	enum LightType
	{
		LightType_PointLight,
		LightType_DirectionalLight,
		LightType_SpotLight,

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

	struct FormatVersion
	{
		unsigned int formatVersionMajor;
		unsigned int formatVersionMinor;

		bool operator ==(const FormatVersion& obj)
		{
			return (this->formatVersionMajor != obj.formatVersionMajor && this->formatVersionMinor != obj.formatVersionMinor);
		}

		bool operator !=(const FormatVersion& obj)
		{
			return !(*this == obj);
		}
	};

	struct ObjectTypeHeader
	{
		ObjectType typeID;
	};

	struct PhysicsObject
	{
		UsePhysics usePhysics;
		float mass;
		float inertiaMagnitude[3];
		float inertiaRotation[3];
		float frictionCoeffStatic;
		float frictionCoeffDynamic;
		CollisionGeometryType geometryType;
	};

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
	};

	struct ObjectHeader : public ObjectTypeHeader, public PhysicsObject
	{
		//Model,
		std::string ModelFile;
		//Position
		float position[3];
		//Rotation
		float rotation[3];
		float angle;
		//Scale
		float scale[3];
	};


	/************************************
				Lights
	*************************************/

	struct BasicLight : public ObjectTypeHeader
	{
		LightType lightType;
		float ambientColor[3];
		float diffuseColor[3];
		float specularColor[3];
	};

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
	};
}

#endif
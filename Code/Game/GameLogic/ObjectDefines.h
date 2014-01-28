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
		//Etc

		ObjectType_NUM_OF_TYPES,

		ObjectType_Unknown = -1,
	};

	enum UsePhysics
	{
		UsePhysics_UseFullPhysics,
		UsePhysics_IgnoreGravity,
		UsePhysics_IgnorePhysics,

		UsePhysics_Count,
		UsePhysics_Unknown = -1,
	};

	//Should this be moved somewhere else?
	enum GameMode
	{
		GameMode_FreeForAll,
		GameMode_TeamDeathMatch,
		//Etc

		GameMode_Count,
		GameMode_Unknown = -1,
	};


	/************************************
				Structs
	*************************************/

	struct FormatVersion
	{
		int formatVersionMajor;
		int formatVersionMinor;

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
		float mass;
		float elasticity;
		float frictionCoeffStatic;
		float frictionCoeffDynamic;
		float inertiaTensor[16];
		UsePhysics usePhysics;
	};

	struct LevelMetaData : ObjectTypeHeader
	{
		std::string levelName;
		FormatVersion levelVersion;
		std::string levelDescription;
		std::string levelAuthor;
		int maxNumberOfPlayer;
		float worldSize;
		int overviewPictureID;
		std::vector<GameMode> gameModesSupported;
	};

	struct ObjectHeader : public PhysicsObject, public ObjectTypeHeader
	{
		//Model,
		int ModelID;
		//Texture
		int TextureID;
		//Position
		float position[3];
		//Rotation
		float rotation[3];
		//Scale
		float scale[3];
	};
}

#endif
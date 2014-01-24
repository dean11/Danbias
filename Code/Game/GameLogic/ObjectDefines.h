#ifndef OBJECT_DEFINES_H
#define OBJECT_DEFINES_H

namespace GameLogic
{
	enum ObjectType
	{
		ObjectType_Level,
		ObjectType_Static,
		ObjectType_Dynamic,


		ObjectType_NUM_OF_TYPES,

		ObjectType_Unknown = -1,
	};

	struct ObjectTypeHeader
	{
		ObjectType typeID;

	};

	struct ObjectHeader : public ObjectTypeHeader
	{
		//Model,
		
		//Texture

		//Position
		float position[3];
		//Rotation
		float rotation[3];
		//Scale
		float scale[3];
	};
}

#endif
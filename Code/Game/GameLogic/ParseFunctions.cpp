//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "ParseFunctions.h"
#include "../../Misc/Packing/Packing.h"
#include <string>

using namespace Oyster::Packing;
using namespace GameLogic::LevelFileLoader;
using namespace GameLogic;
using namespace std;

namespace GameLogic
{
	namespace LevelFileLoader
	{
		ObjectTypeHeader parseObjectTypeHeader(unsigned char* buffer)
		{		
			struct ObjectTypeHeader header;
			int i = Unpacki(buffer);
			header.typeID = (ObjectType)i;
			return header;
		}

		ObjectHeader     parseObjectHeader    (unsigned char* buffer)
		{
		struct ObjectHeader header;
		int x, y,z;
		string s;
		int start = 0;


		//ModelID
		x = Unpacki(buffer);
		header.ModelID = (ObjectType)x;

		//TextureID
		start += 4;
		x = Unpacki(&buffer[start]);
		header.TextureID = x;

		//Position
		start += 4;
		x = Unpacki(&buffer[start]);

		start += 4;
		y = Unpacki(&buffer[start]);

		start += 4;
		z = Unpacki(&buffer[start]);

		header.position[0] = x;
		header.position[1] = y;
		header.position[2] = z;

		//Rotation
		start += 4;
		x = Unpacki(&buffer[start]);

		start += 4;
		y = Unpacki(&buffer[start]);

		start += 4;
		z = Unpacki(&buffer[start]);

		header.rotation[0] = x;
		header.rotation[1] = y;
		header.rotation[2] = z;

		//Scale
		start += 4;
		x = Unpacki(&buffer[start]);

		start += 4;
		y = Unpacki(&buffer[start]);

		start += 4;
		z = Unpacki(&buffer[start]);

		header.scale[0] = x;
		header.scale[1] = y;
		header.scale[2] = z;


		return header;
		}
	}
}
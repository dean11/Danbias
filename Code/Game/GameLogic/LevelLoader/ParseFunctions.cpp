//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "ParseFunctions.h"
#include "../../../Misc/Packing/Packing.h"
#include <string>

using namespace Oyster::Packing;
using namespace GameLogic::LevelFileLoader;
using namespace GameLogic;
using namespace std;

namespace GameLogic
{
	namespace LevelFileLoader
	{
		void ParseObject(unsigned char* buffer, void* header, int size)
		{
			memcpy(header, buffer, size);
		}

		void ParseLevelMetaData(unsigned char* buffer, struct LevelMetaData &header)
		{
			int start = 0;
			int tempSize;
			memcpy(&header.typeID, &buffer[start], 4);
			start += 4;
			memcpy(&tempSize , &buffer[start], 4);
			start += 4;
			memcpy(&header.levelName, &buffer[start], tempSize);
			start += tempSize;
			memcpy(&header.levelVersion, &buffer[start], 8)
			start += 8;
			memcpy(&tempSize, &buffer[start], 4);
			start +=4;
			memcpy(&header.description, &buffer[start], tempSize);
			start += tempSize;
			memcpy(&tempSize, &buffer[start], 4);
			start += 4;
			memcpy(&header.author, &buffer[start], tempSize);
			start += tempSize;
			memcpy(&header.nrOfPlayers, &buffer[start], 4);
			start += 4;
			memcpy(&header.worldSize, &buffer[start], 4);
			start += 4;
			memcpy(&header.map, &buffer[start], 4);
			start += 4;
			memcpy(&tempSize, &buffer[start], 4);
			start += 4;
			memcpy(&header.gameModes, &buffer[start], 4 * tempSize);
			start += tempSize;
		}
	}
}
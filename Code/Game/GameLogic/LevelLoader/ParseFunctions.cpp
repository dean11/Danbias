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
		void ParseObject(char* buffer, void *header, int size)
		{
			memcpy(header, buffer, size);
		}

		void ParseObject(char* buffer, ObjectHeader& header, int& size)
		{
			char tempName[128];
			unsigned int tempSize = 0;
			int start = 0;

			memcpy(&header.typeID, &buffer[start], 4);
			start += 4;

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.ModelFile.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			//3 float[3], 1 float
			memcpy(&header.position, &buffer[start], 40);
			start += 40;

			//2 float[3], 3 float, 2 uint
			memcpy(&header.usePhysics, &buffer[start], 44);
			start += 44;

			size += start;
		}

		void ParseLevelMetaData(char* buffer, LevelMetaData &header, int &size)
		{
			int start = 0;
			unsigned int tempSize;
			char tempName[128];

			memcpy(&header.typeID, &buffer[start], 4);
			start += 4;

			memcpy(&tempSize , &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.levelName.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			memcpy(&header.levelVersion, &buffer[start], 4);
			start += 4;

			memcpy(&tempSize, &buffer[start], 4);
			start +=4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.levelDescription.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.levelAuthor.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			memcpy(&header.maxNumberOfPlayer, &buffer[start], 4);
			start += 4;

			memcpy(&header.worldSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.overviewPicturePath.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			int temp;

			for(int i = 0; i < tempSize; i++)
			{
				memcpy(&temp, &buffer[start], 4);
				start += 4;
				header.gameModesSupported.push_back((GameMode)temp);
			}

			size += start;
		}
	}
}
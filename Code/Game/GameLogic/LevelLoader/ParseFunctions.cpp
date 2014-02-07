//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "ParseFunctions.h"
#include "../../../Misc/Packing/Packing.h"
#include "Loader.h"
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

			memcpy(&header.specialTypeID, &buffer[start], 4);
			start += 4;

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);
			header.ModelFile.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			//The reset of the object struct
			//3 float[3], 1 float
			memcpy(&header.position, &buffer[start], 40);
			start += 40;

			//Physics struct
			//2 float[3], 4 float, 1 uint
			memcpy(&header.usePhysics, &buffer[start], 44);
			start += 44;
			
			//Read path for bounding volume
			ParseBoundingVolume(&buffer[start], header.boundingVolume, start);

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

		void ParseBoundingVolume(char* buffer, LevelLoaderInternal::BoundingVolume& volume, int &size)
		{
			int start = 0;
			int tempSize = 0;
			char tempName[128];

			memcpy(&tempSize, &buffer[start], 4);
			start += 4;

			memcpy(&tempName, &buffer[start], tempSize);

			string fileName;
			fileName.assign(&tempName[0], &tempName[tempSize]);
			start += tempSize;

			//L�s in filen.
			int fileLength = 0;
			Loader loader;
			char* buf = loader.LoadFile("E:\\Dropbox\\Programming\\Github\\Danbias\\Bin\\Content\\Worlds\\cgf\\"+ fileName, fileLength);
			
			LevelLoaderInternal::FormatVersion version;
			memcpy(&version, &buffer[0], sizeof(version));

			memcpy(&volume.geoType, &buf[8], sizeof(volume.geoType));
			//start += sizeof(volume.geoType);

			switch(volume.geoType)
			{
			case CollisionGeometryType_Box:
				memcpy(&volume.box, &buf[12], sizeof(volume.box));
				//start += sizeof(volume.box);
				break;

			case CollisionGeometryType_Sphere:
				memcpy(&volume.sphere, &buf[12], sizeof(volume.sphere));
				//start += sizeof(volume.sphere);
				break;

			case CollisionGeometryType_Cylinder:
				memcpy(&volume.cylinder, &buf[12], sizeof(volume.cylinder));
				//start += sizeof(volume.cylinder);
				break;

			default:
				break;
			}

			size += start;
		}
	}
}
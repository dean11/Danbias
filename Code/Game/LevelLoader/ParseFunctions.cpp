//////////////////////////////////
// Created by Sam Svensson 2013 //
//////////////////////////////////

#include "ParseFunctions.h"
#include "Loader.h"
#include <string>

using namespace GameLogic::LevelFileLoader;
using namespace GameLogic;
using namespace std;

namespace GameLogic
{
	namespace LevelFileLoader
	{
		//can parse any struct if the struct doesnt contain strings or char[]
		void ParseObject(char* buffer, void *header, int size)
		{
			memcpy(header, buffer, size);
		}

		void ParseLight(char* buffer, BasicLight& header, int& size)
		{
			int start = 0;
			memcpy(&header.typeID, &buffer[start], 4);
			start += 4;

			memcpy(&header.lightType, &buffer[start], 4);
			start += 4;
			
			memcpy(&header.color, &buffer[start], 12);
			start += 12;
			
			memcpy(&header.position, &buffer[start], 12);
			start += 12;
			
			memcpy(&header.radius, &buffer[start], 4);
			start += 4;
			
			memcpy(&header.intensity, &buffer[start], 4);
			start += 4;

			size += start;

			//memcpy(&header, buffer, size);
		}

		void ParseObject(char* buffer, ObjectHeader& header, int& size, bool loadCgf)
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
			
			//if loadCgf : Read path for bounding volume
			if(loadCgf)
			{
				ParseBoundingVolume(&buffer[start], header.boundingVolume, start);
			}

			//else make sure the counter counts the name so we can jump over the string in the buffer. 
			else
			{
				memcpy(&tempSize, &buffer[start], 4);
				start += 4;

				memcpy(&tempName, &buffer[start], tempSize);

				string fileName;
				fileName.assign(&tempName[0], &tempName[tempSize]);
				start += tempSize;
			}

/*			if(header.boundingVolume.sphere.mass == 0)
			{
				header.typeID = ObjectType_Static;
			}
			else
			{
				header.typeID = ObjectType_Dynamic;
			}
			*/
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

			for(int i = 0; i < (int)tempSize; i++)
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

			size += start;

			//Läs in filen.
			int fileLength = 0;
			Loader loader;
			char* buf = loader.LoadFile("../Content/Worlds/cgf/"+ fileName, fileLength);

			if(buf == NULL)
			{
				printf("Could not find %s\n", fileName.c_str());
				return;	//Could not find cgf file
			}
			start = 0;
			LevelLoaderInternal::FormatVersion version;
			memcpy(&version, &buf[0], sizeof(version));
			start += 8;

			memcpy(&volume.geoType, &buf[start], sizeof(volume.geoType));

			switch(volume.geoType)
			{
			case CollisionGeometryType_Box:
				memcpy(&volume.box, &buf[start], sizeof(volume.box));
				start += sizeof(volume.box);
				break;

			case CollisionGeometryType_Sphere:
				memcpy(&volume.sphere, &buf[start], sizeof(volume.sphere));
				start += sizeof(volume.sphere);
				break;

			case CollisionGeometryType_Cylinder:
				memcpy(&volume.cylinder, &buf[start], sizeof(volume.cylinder));
				start += sizeof(volume.cylinder);
				break;

			case CollisionGeometryType_CG_MESH:
				{
					memcpy(&volume.cgMesh, &buf[start], sizeof(float)*12);
					start += sizeof(float)*12;
					memcpy(&tempSize, &buf[start], sizeof(tempSize));
					start += 4;
					memcpy(&tempName, &buf[start], tempSize);
					tempName[tempSize] = '\0';

					//convert from char[] to wchar_t[]
					mbstowcs_s(NULL, volume.cgMesh.filename, tempSize+1, tempName, _TRUNCATE);

					start += tempSize;
				}
				break;

			default:
				break;
			}
		}
	}
}
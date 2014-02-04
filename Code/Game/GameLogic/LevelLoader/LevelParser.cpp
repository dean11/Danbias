#include "LevelParser.h"

#include "Loader.h"
#include "ParseFunctions.h"

using namespace GameLogic;
using namespace ::LevelFileLoader;
using namespace Utility::DynamicMemory;

LevelParser::LevelParser()
{
	formatVersion.formatVersionMajor = 1;
	formatVersion.formatVersionMinor = 0;
}

LevelParser::~LevelParser()
{
}

std::vector<SmartPointer<ObjectTypeHeader>> LevelParser::Parse(std::string filename)
{
	int bufferSize = 0;
	int counter = 0;

	std::vector<SmartPointer<ObjectTypeHeader>> objects;

	//Read entire level file.
	Loader loader;
	char* buffer = (char*)loader.LoadFile(filename.c_str(), bufferSize);

	//Read format version
	FormatVersion levelFormatVersion;
	ParseObject(&buffer[counter], &levelFormatVersion, sizeof(levelFormatVersion));
	counter += sizeof(levelFormatVersion);
	if(this->formatVersion != levelFormatVersion)
	{
		//Do something if it's not the same version
	}

  	while(counter < bufferSize)
	{
		//Get typeID
		ObjectTypeHeader typeID;
		ParseObject(&buffer[counter], &typeID, sizeof(typeID));
		switch((int)typeID.typeID)
		{
			case ObjectType_LevelMetaData:
			{
				LevelMetaData* header = new LevelMetaData;
				ParseLevelMetaData(&buffer[counter], *header, counter);
				objects.push_back(header);
				break;
			}

			//This is by design, static and dynamic is using the same converter. Do not add anything inbetween them.
			case ObjectType_Static: case ObjectType_Dynamic:
			{
				ObjectHeader* header = new ObjectHeader;
				ParseObject(&buffer[counter], *header, counter);
				objects.push_back(header);
				break;
			}
			
			case ObjectType_Light:
			{
				LightType lightType;

				//Get Light type
				ParseObject(&buffer[counter+4], &lightType, sizeof(lightType));

				switch(lightType)
				{
				case LightType_PointLight:
				{
					PointLight* header = new PointLight;
					ParseObject(&buffer[counter], header, sizeof(*header));
					counter += sizeof(*header);
					objects.push_back(header);
					break;
				}
				case LightType_DirectionalLight:
				{
					DirectionalLight* header = new DirectionalLight;
					ParseObject(&buffer[counter], header, sizeof(*header));
					counter += sizeof(*header);
					objects.push_back(header);
					break;
				}
				case LightType_SpotLight:
				{
					SpotLight* header = new SpotLight;
					ParseObject(&buffer[counter], header, sizeof(*header));
					counter += sizeof(*header);
					objects.push_back(header);
					break;
				}
				default:
					//Undefined LightType.
					break;
				}
				break;
			}
			default:
				//Couldn't find typeID. FAIL!!!!!!
				break;
		}
	}

	return objects;
}

//för meta information om leveln.
LevelMetaData LevelParser::ParseHeader(std::string filename)
{
	int bufferSize = 0;
	int counter = 0;

	LevelMetaData levelHeader;
	levelHeader.typeID = ObjectType::ObjectType_Unknown;

	//Read entire level file.
	Loader loader;
	char* buffer = (char*)loader.LoadFile(filename.c_str(), bufferSize);

	//Read format version
	FormatVersion levelFormatVersion;
	ParseObject(&buffer[counter], &levelFormatVersion, sizeof(formatVersion));
	counter += sizeof(levelFormatVersion);
	if(this->formatVersion != levelFormatVersion)
	{
		//Do something if it's not the same version
	}

	//Find the header in the returned string.
	while(counter < bufferSize)
	{
		ObjectTypeHeader typeID;
		ParseObject(&buffer[counter], &typeID, sizeof(typeID));

		switch(typeID.typeID)
		{
		case ObjectType_LevelMetaData:
			ParseLevelMetaData(&buffer[counter], levelHeader, counter);
			return levelHeader;
			break;
		
			//This is by design, static and dynamic is using the same converter. Do not add anything inbetween them.
		case ObjectType_Static: case ObjectType_Dynamic:
		{
			ObjectHeader header;
			ParseObject(&buffer[counter], header, counter);
			break;
		}

		case ObjectType_Light:
		{
			LightType lightType;
			ParseObject(&buffer[counter+4], &lightType, sizeof(lightType));

			switch(lightType)
			{
			case LightType_PointLight:
			{
				counter += sizeof(PointLight);
				break;
			}
			case LightType_DirectionalLight:
			{
				counter += sizeof(DirectionalLight);
				break;
			}
			case LightType_SpotLight:
			{
				counter += sizeof(SpotLight);
				break;
			}
			default:
				//Undefined LightType.
				break;
			}
		}

		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return levelHeader;
}
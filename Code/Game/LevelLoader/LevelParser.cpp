/////////////////////////////////////
// Created by Pontus Fransson 2013 //
/////////////////////////////////////

#include "LevelParser.h"

#include "Loader.h"
#include "ParseFunctions.h"

using namespace GameLogic;
using namespace ::LevelFileLoader;
using namespace Utility::DynamicMemory;

LevelParser::LevelParser()
{
	formatVersion.formatVersionMajor = 3;
	formatVersion.formatVersionMinor = 0;
}

LevelParser::~LevelParser()
{
}

std::vector<SmartPointer<ObjectTypeHeader>> LevelParser::Parse(std::string filename)
{
	int bufferSize = 0;
	int counter = 0;
	bool loadCgf;

	std::vector<SmartPointer<ObjectTypeHeader>> objects;

	//Read entire level file.
	Loader loader;
	char* buffer = (char*)loader.LoadFile(filename.c_str(), bufferSize);

	if(buffer)
	{
		//Read format version
		LevelLoaderInternal::FormatVersion levelFormatVersion;
		ParseObject(&buffer[counter], &levelFormatVersion, sizeof(levelFormatVersion));
		counter += sizeof(levelFormatVersion);
		if(this->formatVersion != levelFormatVersion)
		{
			//Returns an empty vector, because it will most likely fail to read the level format.
			return objects;
		}

  		while(counter < bufferSize)
		{
			loadCgf = true;
			//Get typeID
			ObjectType typeID;
			ParseObject(&buffer[counter], &typeID, sizeof(typeID));
			switch((int)typeID)
			{
				case ObjectType_LevelMetaData:
				{
					SmartPointer<ObjectTypeHeader> header = new LevelMetaData;
					ParseLevelMetaData(&buffer[counter], *(LevelMetaData*)header.Get(), counter);
					objects.push_back(header);
					break;
				}

				case ObjectType_SpawnPoint:
				{
					loadCgf = false;
					ObjectHeader* header = new ObjectHeader;
					ParseObject(&buffer[counter], *header, counter, loadCgf);

					SpawnPointAttributes* spawn = new SpawnPointAttributes;

					spawn->typeID = header->typeID;

					for(int i = 0; i < 3; i++)
					{
						spawn->position[i] = header->position[i];
					}

					delete header;
					//objects.push_back(header);
					objects.push_back(spawn);
					break;
				}

				//This is by design, static and dynamic is using the same converter. Do not add anything inbetween them. 
				//Unless they are changed to not be the same.
				case ObjectType_Static: case ObjectType_Dynamic:
				{
					//Get specialType.
					ObjectSpecialType specialType;
					ParseObject(&buffer[counter+4], &specialType, sizeof(typeID));

					switch(specialType)
					{
						case ObjectSpecialType_None:
						{
							ObjectHeader* header = new ObjectHeader;
							ParseObject(&buffer[counter], *header, counter, false);
							objects.push_back(header);

							break;
						}

						//These does not have any cgf file.
						case ObjectSpecialType_NonSolidRotationQuick:
						case ObjectSpecialType_NonSolidRotationSlow:
						{
							ObjectHeader* header = new ObjectHeader;
							ParseObject(&buffer[counter], *header, counter, false);
							objects.push_back(header);

							break;
						}

						//there is no difference when parsing these specialTypes.
						case ObjectSpecialType_LightSource:
						case ObjectSpecialType_PickupEnergy:
						case ObjectSpecialType_Generic:
						case ObjectSpecialType_PickupHealth:
						case ObjectSpecialType_CrystalShard:
						case ObjectSpecialType_CrystalFormation:
						case ObjectSpecialType_Spike:
						case ObjectSpecialType_SpikeBox:
						case ObjectSpecialType_RedExplosiveBox:
						case ObjectSpecialType_StandardBox:
						case ObjectSpecialType_Stone:
						case ObjectSpecialType_Building:
						{
							ObjectHeader* header = new ObjectHeader;
							ParseObject(&buffer[counter], *header, counter, loadCgf);
							objects.push_back(header);

							break;
						}

						case ObjectSpecialType_JumpPad:
						{
							JumpPadAttributes* header = new JumpPadAttributes;
							ParseObject(&buffer[counter], *header, counter, loadCgf);

							//Read the spec
							ParseObject(&buffer[counter], header->direction, 16);
							counter += 16;
							objects.push_back(header);

							break;
						}
		
						case ObjectSpecialType_Portal:
						{
							PortalAttributes* header = new PortalAttributes;
							ParseObject(&buffer[counter], *header, counter, loadCgf);

							ParseObject(&buffer[counter], header->destination, 12);
							counter += 12;
							objects.push_back(header);

							break;
						}
					
						case ObjectSpecialType_World:
						{
							WorldAttributes* header = new WorldAttributes;
							ParseObject(&buffer[counter], *header, counter, loadCgf);

							ParseObject(&buffer[counter], &header->worldSize, 8);
							counter += 8;
							objects.push_back(header);
							break;
						}

						//this is a hotfix, fix so you only load the relevant data when the file is updated
						default:
							//Couldn't find specialType
							break;
					}
					break;
				}
			
				case ObjectType_Light:
				{
					//LightType lightType;

					//Get Light type
					//ParseObject(&buffer[counter+4], &lightType, sizeof(lightType));

					//We only support PointLight for now.
					BasicLight* header = new BasicLight;
					
					ParseLight(&buffer[counter], *header, counter);
					objects.push_back(header);

					/*switch(lightType)
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
					break;*/
				}
				break;
				default:
					//Couldn't find typeID. FAIL!!!!!!
					break;
			}
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
	LevelLoaderInternal::FormatVersion levelFormatVersion;
	ParseObject(&buffer[counter], &levelFormatVersion, sizeof(formatVersion));
	counter += sizeof(levelFormatVersion);
	if(this->formatVersion != levelFormatVersion)
	{
		//Do something if it's not the same version

		//Returns an empty levelHeader with ObjectType_Unknown.
		//Because it will not be able to read another version of the level format.
		return levelHeader;
	}

	//Find the header in the returned string.
	while(counter < bufferSize)
	{
		ObjectType typeID;
		ParseObject(&buffer[counter], &typeID, sizeof(typeID));

		switch(typeID)
		{
		case ObjectType_LevelMetaData:
			ParseLevelMetaData(&buffer[counter], levelHeader, counter);
			return levelHeader;
			break;
		
			//This is by design, static and dynamic is using the same converter. Do not add anything inbetween them.
		case ObjectType_Static: case ObjectType_Dynamic:
		{
			ObjectHeader header;
			ParseObject(&buffer[counter], &header, counter);

			switch(header.specialTypeID)
			{
			case ObjectSpecialType_JumpPad:
				counter += sizeof(16);
				break;
			case ObjectSpecialType_Portal:
				counter += sizeof(12);
				break;
			default:
				break;
			}	
			break;
		}

		case ObjectType_Light:
		{
			LightType lightType;
			ParseObject(&buffer[counter+4], &lightType, sizeof(lightType));

			//We only support pointlight for now.
			counter += sizeof(BasicLight);
			/*
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
			}*/
		}

		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return levelHeader;
}
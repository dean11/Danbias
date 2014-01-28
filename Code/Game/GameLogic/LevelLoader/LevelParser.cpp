#include "LevelParser.h"

#include "Loader.h"
#include "ParseFunctions.h"

using namespace GameLogic;
using namespace ::LevelFileLoader;

LevelParser::LevelParser()
{
	formatVersion.formatVersionMajor = 1;
	formatVersion.formatVersionMinor = 0;
}

LevelParser::~LevelParser()
{
}

std::vector<ObjectTypeHeader> LevelParser::Parse(std::string filename)
{
	int bufferSize = 0;
	unsigned int counter = 0;

	std::vector<ObjectTypeHeader> objects;

	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str(), bufferSize);

	//Read format version
	FormatVersion levelFormatVersion;
	ParseObject(&buffer[counter], formatVersion, sizeof(formatVersion));
	if(this->formatVersion != levelFormatVersion)
	{
		//Do something if it's not the same version
	}

	while(counter < bufferSize)
	{
		//Get typeID
		ObjectTypeHeader typeID;
		ParseObject(&buffer[counter], typeID, sizeof(typeID));

		switch((int)typeID.typeID)
		{
		case ObjectType_LevelMetaData:
			LevelMetaData header;
			//ParseObject(&buffer[counter], header, sizeof(header));
			objects.push_back(header);
			counter += sizeof(header);
			break;

		case ObjectType_Dynamic:
			ObjectHeader header;
			ParseObject(&buffer[counter], header, sizeof(header));
			objects.push_back(header);
			counter += sizeof(header);
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
	unsigned int counter = 0;

	LevelMetaData levelHeader;
	levelHeader.typeID = ObjectType::ObjectType_Unknown;

	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str(), bufferSize);

	//Read format version
	FormatVersion levelFormatVersion;
	ParseObject(&buffer[counter], formatVersion, sizeof(formatVersion));
	if(this->formatVersion != levelFormatVersion)
	{
		//Do something if it's not the same version
	}

	//Find the header in the returned string.
	while(counter < bufferSize)
	{
		ObjectTypeHeader typeID;
		ParseObject(&buffer[counter], typeID, sizeof(typeID));

		switch(typeID.typeID)
		{
		case ObjectType_LevelMetaData:
			//ParseObject(&buffer[counter], levelHeader, sizeof(levelHeader));
			return levelHeader;
			counter += sizeof(LevelMetaData);
			break;
		case ObjectType_Dynamic:
			//Do not call parse this object, since we are only interested in the LevelMetaData
			//Only increase the counter size
			counter += sizeof(ObjectHeader);
			break;

		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return levelHeader;
}
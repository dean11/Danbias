#include "LevelParser.h"

#include "Loader.h"
#include "ParseFunctions.h"

using namespace GameLogic;
using namespace ::LevelFileLoader;


LevelParser::LevelParser()
{
}

LevelParser::~LevelParser()
{
}

std::vector<ObjectTypeHeader> LevelParser::Parse(std::string filename)
{
	int stringSize = 0;
	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str(), stringSize);

	std::vector<ObjectTypeHeader> objects;

	unsigned int counter = 0;
	
	while(counter < stringSize)
	{
		//Get typeID
		ObjectTypeHeader typeID;
		typeID = ParseObjectTypeHeader(&buffer[counter]);
		//counter += 4;
		//Unpack ID

		switch((int)typeID.typeID)
		{
		case TypeID_LevelHeader:
			//Call function
			counter += LevelHeaderSize;
			break;

		case TypeID_Object:
			//Call function
			objects.push_back(ParseObjectHeader(&buffer[counter]));
			counter += sizeof(ObjectHeader);
			break;
		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return objects;
}

//för meta information om leveln. Måste göra så den returnerar rätt strukt så fort vi
//vi definierat en!
ObjectTypeHeader LevelParser::ParseHeader(std::string filename)
{
	int stringSize = 0;
	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str(), stringSize);

	//Find the header in the returned string.
	unsigned int counter = 0;
	

	ObjectTypeHeader header;
	header.typeID = ObjectType_Level;

	while(counter < stringSize)
	{
		ObjectTypeHeader typeID;
		typeID = ParseObjectTypeHeader(buffer);
		switch(typeID.typeID)
		{
		case TypeID_LevelHeader:
			//Call function

			counter += LevelHeaderSize;
			break;
		case TypeID_Object:
			//Call function
			counter += ObjectHeaderSize;
			break;

		default:
			//Couldn't find typeID. FAIL!!!!!!
			break;
		}
	}

	return header;
}
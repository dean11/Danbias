#include "LevelParser.h"

#include "Loader.h"

using namespace GameLogic;
using namespace ::LevelFileLoader;

LevelParser::LevelParser()
{
}

LevelParser::~LevelParser()
{
}

//
std::vector<ObjectTypeHeader> LevelParser::Parse(std::string filename)
{
	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str());

	std::vector<ObjectTypeHeader> objects;

	unsigned int counter = 0;
	unsigned int stringSize = 0;
	while(counter < stringSize)
	{
		//Get typeID
		int typeID = 0;
		
		//Unpack ID

		switch(typeID)
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

	return objects;
}

//
ObjectTypeHeader LevelParser::ParseHeader(std::string filename)
{
	//Read entire level file.
	Loader loader;
	unsigned char* buffer = (unsigned char*)loader.LoadFile(filename.c_str());

	//Find the header in the returned string.
	unsigned int counter = 0;
	unsigned int stringSize = 0;

	ObjectTypeHeader header;
	header.typeID = ObjectType_Level;

	while(counter < stringSize)
	{
		int typeID = 0;
		switch(typeID)
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